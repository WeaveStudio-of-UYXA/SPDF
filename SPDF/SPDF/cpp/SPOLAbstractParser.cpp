#include "../SPOLAbstractParser.h"
#include "../SPOLInterpreter.h"

def_init SPOLAbstractControllerParser::SPOLAbstractControllerParser() {
	Interpreter = nullptr;
	ControllerFlag = "Unknown";
	FunctionLike = false;
}

bool SPOLAbstractControllerParser::parseLine(const QString& line, SPDF::SPOLExecutionMode mode) {
	ExecutionMode = mode;
	int cnLength = ControllerFlag.length();
	QString pureLine = "";
	if (FunctionLike) {
		pureLine = line.mid(cnLength + 1, line.length() - cnLength - 2);
		return onParseLine(pureLine, mode);
	}
	else {
		pureLine = line.mid(cnLength, line.length() - cnLength);
		return onParseLine(pureLine, mode);
	}
}

void SPOLAbstractControllerParser::recordParserResult(const SPDFControllerData& result) {
	Parameters.append(result);
}

unsigned long long SPOLAbstractControllerParser::getExecuteLineIndex() {
	return Interpreter->getExecuteLineIndex();
}

void SPOLAbstractControllerParser::changeExecuteLine(unsigned int index) {
	Interpreter->changeExecuteLine(index);
}

void SPOLAbstractControllerParser::setVariable(const QString& name, const QVariant& value) {
	Interpreter->setVariable(name, value);
}

QVariant SPOLAbstractControllerParser::getVariable(const QString& name) {
	return Interpreter->getVariable(name);
}

QString SPOLAbstractControllerParser::getSPOLWithIndex(unsigned int index) {
	return Interpreter->getSPOLWithIndex(index);
}

SPDFControllerDataList* SPOLAbstractControllerParser::getParameters() {
	return &Parameters;
}

def_init  SPOLVIESParserContainer::SPOLVIESParserContainer(QJSValue parser) {
	Engine = parser.engine();
	VIES_SPDFParser = parser;
	VIES_onParseLine = parser.property("onParseLine");
	ControllerFlag = parser.property("ControllerFlag").toString();
	FunctionLike = parser.property("FunctionLike").toBool();
}

bool SPOLVIESParserContainer::onParseLine(const QString& line, SPDF::SPOLExecutionMode mode) {
	QJSValueList args;
	args << line;
	args << (int)mode;
	QJSValue ParaMapList = Engine->newArray();
	VIES_SPDFParser.setProperty("Parameters", ParaMapList);
	QJSValue result = VIES_onParseLine.callWithInstance(VIES_SPDFParser, args);
	return result.toBool();
}

SPDFControllerDataList* SPOLVIESParserContainer::getParameters() {
	QJSValue ParaMapList = VIES_SPDFParser.property("Parameters");
	int length = ParaMapList.property("length").toInt();
	for (auto i = 0; i < length; i++) {
		QJSValue VIESObj = ParaMapList.property(i);
		SPDFControllerData obj;
		obj.MethodName = VIESObj.property("MethodName").toString();
		obj.NoWait = VIESObj.property("NoWait").toBool();
		obj.Parameters = VIESObj.property("Parameters").toVariant().toMap();
		Parameters.append(obj);
	}
	return &Parameters;
}

def_init SPOLStandardControllerParser::SPOLStandardControllerParser() {
	ControllerJSON = new VIDocument::VIJSON();
}
void SPOLStandardControllerParser::setControllerJSON(const QString& json) {
	if (ControllerJSON->loadSettings(json)) {
		ControllerFlag = ControllerJSON->getValueOf("Tag").toString();
		FunctionLike = ControllerJSON->getValueOf("Function").toBool();
		BottomInterfaceName = ControllerJSON->getValueOf("Name").toString();
		Count = ControllerJSON->getValueOf("Count").toInt();
		if (FunctionLike) { Count = 1; }
	}
	else {
		ControllerFlag = "Unknown";
		FunctionLike = false;
		BottomInterfaceName = "Unknown";
		Count = 0;
	}
}
bool SPOLStandardControllerParser::onParseLine(const QString& line, SPDF::SPOLExecutionMode mode) {
	SPDFControllerData data;
	data.MethodName = BottomInterfaceName;
	data.NoWait = ! ControllerJSON->getValueOf("Waiting").toBool();
	if (Count != 1) {
		QStringList topPart = line.split(ControllerFlag);
		data.Parameters.insert("Count", QString::number(topPart.size()));
		for (int i = 0; i < topPart.size(); i++) {
			QString subPart = topPart[i];
			bool success = onParseLine(subPart, QString::number(i), &data);
			if (!success) {
				return false;
			}
		}
	}
	else {
		bool success = onParseLine(line, "", &data);
		if (!success) {
			return false;
		}
	}
	recordParserResult(data);
}
bool SPOLStandardControllerParser::onParseLine(const QString & line, QString rootJSON, SPDFControllerData* data){
	QString splitChar = ControllerJSON->getValueOf("Split").toString();
	if (splitChar == "") { //which means ParaTag Mode
		QStringList list = VICommandHost::scientificSplitter(line, '-');
		QStringList parts = ControllerJSON->getKeysOf("SubPart");
		if (parts.length() != 1) {
			consoleLog("Error: SubPart Count Error. ParaTag Mode SubPart Count must be 1");
			return false;
		}
		int paraLength = list[0].toInt();
		QMap<QString, QString> paraDefMap;
		for (int i = 0; i < paraLength; i++) {
			QString paraName = "";
			QString paraDefault = "";
			QString fullKey = "SubPart." + QString::number(paraLength) + "." + QString::number(i);
			if (ControllerJSON->getKeysOf(fullKey).length() == 0) {
				paraName = ControllerJSON->getValueOf("CustomStruct."+ControllerJSON->getValueOf(fullKey).toString()+".Name").toString();
				paraDefault = ControllerJSON->getValueOf("CustomStruct." + ControllerJSON->getValueOf(fullKey).toString() + ".Default").toString();
			}
			else {
				paraName = ControllerJSON->getValueOf(fullKey+".Name").toString();
				paraDefault = ControllerJSON->getValueOf(fullKey + ".Default").toString();
			}
			paraDefMap.insert(paraName, paraDefault);
		}
		for (auto i : list) {
			QString tpara = i.trimmed();
			QStringList para = VICommandHost::scientificSplitter(tpara, ' ');
			if (para.length() > 2 || para.length() < 1) {
				consoleLog("Error: ParaTag Syntax Error. ParaTag needs a key and a value");
				return false;
			}
			QString paraName = para[0];
			if (!paraDefMap.contains(paraName)) {
				consoleLog("Error: Excepted ParaTag Name: " + paraName);
				return false;
			}
			QString paraValue = "";
			if (para.length() == 2) {
				paraValue = para[1];
			}
			else {
				paraValue = paraDefMap[paraName];
			}
			if (rootJSON == "") {
				data->Parameters.insert(paraName, paraValue);
			}
			else {
				data->Parameters.insert(rootJSON + "." + paraName, paraValue);
			}
		}
	}
	else { // which means split mode
		QStringList list = VICommandHost::scientificSplitter(line, splitChar[0]);
		QStringList parts = ControllerJSON->getKeysOf("SubPart");
		if (parts.length() == 1) {  // when only one subpart length is defined, allow subpart length no more than defined length
			if (list.size() > parts[0].toInt()) {
				consoleLog("Error: SubPart Count Error. SubPart Count must no more than " + parts[0]);
				return false;
			}
			int paraDefLength = parts[0].toInt();
			for (int i = 0; i < paraDefLength; i++) {
				QString fullKey = "SubPart." + QString::number(paraDefLength) + "." + QString::number(i);
				QString part = i >= list.length() ? "" : list[i];
				if (ControllerJSON->getKeysOf(fullKey).size() == 0) {
					parseSubPart(part, ControllerJSON->getValueOf(fullKey).toString(), rootJSON, data);
				}
				else {
					part = part == "" ? ControllerJSON->getValueOf(fullKey + ".Default").toString() : part;
					if (rootJSON == "") {
						data->Parameters.insert(ControllerJSON->getValueOf(fullKey + ".Name").toString(), part);
					}
					else {
						data->Parameters.insert(rootJSON + "." + ControllerJSON->getValueOf(fullKey + ".Name").toString(), part);
					}
				}
			}
		}
		else { // when more than one subpart length is defined, subpart length must equal to defined length
			if (!parts.contains(QString::number(list.length()))) {
				consoleLog("Error: SubPart Count Error. SubPart Count must be " + parts.join(" or "));
				return false;
			}
			int paraDefLength = list.length();
			QString rtJSON;
			if (rootJSON == "") {
				rtJSON = QString::number(paraDefLength);
			}
			else {
				rtJSON = rootJSON + "." + QString::number(paraDefLength);
			}
			for (int i = 0; i < paraDefLength; i++) {
				QString fullKey = "SubPart." + QString::number(paraDefLength) + "." + QString::number(i);
				QString part = i >= list.length() ? "" : list[i];
				if (ControllerJSON->getKeysOf(fullKey).size() == 0) {
					parseSubPart(part, ControllerJSON->getValueOf(fullKey).toString(), rtJSON, data);
				}
				else {
					part = part == "" ? ControllerJSON->getValueOf(fullKey + ".Default").toString() : part;
					if (rtJSON == "") {
						data->Parameters.insert(ControllerJSON->getValueOf(fullKey + ".Name").toString(), part);
					}
					else {
						data->Parameters.insert(rtJSON + "." + ControllerJSON->getValueOf(fullKey + ".Name").toString(), part);
					}
				}
			}
		}
	}
	return true;
}
bool SPOLStandardControllerParser::parseSubPart(const QString& part, const QString& structName, QString rootJSON, SPDFControllerData* data) {
	QStringList keys = ControllerJSON->getKeysOf("CustomStruct." + structName);
	if (keys.length() == 2 && keys.contains("Name") && keys.contains("Default")) {//just a simple value struct
		QString para = part == "" ? ControllerJSON->getValueOf("CustomStruct." + structName + ".Default").toString() : part;
		if (rootJSON == "") {
			data->Parameters.insert(ControllerJSON->getValueOf("CustomStruct." + structName + ".Name").toString(), para);
		}
		else {
			data->Parameters.insert(rootJSON + "." + ControllerJSON->getValueOf("CustomStruct." + structName + ".Name").toString(), para);
		}
	}
	else {//sub split struct
		if (rootJSON == "") {
			rootJSON = ControllerJSON->getValueOf("CustomStruct." + structName + ".Name").toString();
		}
		else {
			rootJSON = rootJSON + "." + ControllerJSON->getValueOf("CustomStruct." + structName + ".Name").toString();
		}
		QString splitChar = ControllerJSON->getValueOf("CustomStruct." + structName + ".Split").toString();
		QString para = part == "" ? ControllerJSON->getValueOf("CustomStruct." + structName + ".Default").toString() : part;
		QStringList list = VICommandHost::scientificSplitter(para, splitChar[0]);
		QStringList parts = ControllerJSON->getKeysOf("CustomStruct." + structName + ".SubPart");
		if (parts.length() == 1) {  // when only one subpart length is defined, allow subpart length no more than defined length
			if (list.size() > parts[0].toInt()) {
				consoleLog("Error: SubPart Count Error. SubPart Count must no more than " + parts[0]);
				return false;
			}
			int paraDefLength = parts[0].toInt();
			for (int i = 0; i < paraDefLength; i++) {
				QString fullKey = "CustomStruct." + structName + ".SubPart." + QString::number(paraDefLength) + "." + QString::number(i);
				QString part = i >= list.length() ? "" : list[i];
				if (ControllerJSON->getKeysOf(fullKey).size() == 0) {
					parseSubPart(part, ControllerJSON->getValueOf(fullKey).toString(), rootJSON, data);
				}
				else {
					part = part == "" ? ControllerJSON->getValueOf(fullKey + ".Default").toString() : part;
					if (rootJSON == "") {
						data->Parameters.insert(ControllerJSON->getValueOf(fullKey + ".Name").toString(), part);
					}
					else {
						data->Parameters.insert(rootJSON + "." + ControllerJSON->getValueOf(fullKey + ".Name").toString(), part);
					}
				}
			}
		}
		else { // when more than one subpart length is defined, subpart length must equal to defined length
			if (!parts.contains(QString::number(list.length()))) {
				consoleLog("Error: SubPart Count Error. SubPart Count must be " + parts.join(" or "));
				return false;
			}
			int paraDefLength = list.length();
			QString rtJSON;
			if (rootJSON == "") {
				rtJSON = QString::number(paraDefLength);
			}
			else {
				rtJSON = rootJSON + "." + QString::number(paraDefLength);
			}
			for (int i = 0; i < paraDefLength; i++) {
				QString fullKey = "CustomStruct." + structName + ".SubPart." + QString::number(paraDefLength) + "." + QString::number(i);
				QString part = i >= list.length() ? "" : list[i];
				if (ControllerJSON->getKeysOf(fullKey).size() == 0) {
					parseSubPart(part, ControllerJSON->getValueOf(fullKey).toString(), rtJSON, data);
				}
				else {
					part = part == "" ? ControllerJSON->getValueOf(fullKey + ".Default").toString() : part;
					if (rtJSON == "") {
						data->Parameters.insert(ControllerJSON->getValueOf(fullKey + ".Name").toString(), part);
					}
					else {
						data->Parameters.insert(rtJSON + "." + ControllerJSON->getValueOf(fullKey + ".Name").toString(), part);
					}
				}
			}
		}
	}
	return true;
}
def_init SPOLControllerParserManager::SPOLControllerParserManager(VISuper* super):VIObject(super) {
	MaxControllerFlagLength = 0;
	ParsersCount = 0;
}

void SPOLControllerParserManager::addSPOLParser(SPOLAbstractControllerParser* parser) {
	QString flag = parser->getControllerFlag();
	consoleLog("SPOLControllerParserManager: Adding SPOL Parser: " + flag);
	int length = flag.length();
	if (length > MaxControllerFlagLength) {
		MaxControllerFlagLength = length;
	}
	consoleLog("SPOLControllerParserManager: Max Controller Flag Length: " + QString::number(MaxControllerFlagLength));

	if (Parsers.contains(length)) {
		if (Parsers[length].contains(parser)) {
			return;
		}
		else {
			Parsers[length].append(parser);
		}
	}
	else {
		QList<SPOLAbstractControllerParser*> list;
		list.append(parser);
		Parsers.insert(length, list);
	}
	setParsersCount(deriveParsersCount());
}

SPOLAbstractControllerParser* SPOLControllerParserManager::identifyFlag(const QString& spol) {
	for (auto i = MaxControllerFlagLength; i > 0; i--) {
		if (Parsers.contains(i)) {
			for (auto parser : Parsers[i]) {
				if (spol.startsWith(parser->getControllerFlag())) {
					consoleLog("SPOLControllerParserManager: Identified SPOL: " + parser->getControllerFlag());
					return parser;
				}
			}
		}
	}
	return nullptr;
}

int SPOLControllerParserManager::deriveParsersCount() {
	int count = 0;
	for (auto i = MaxControllerFlagLength; i > 0; i--) {
		if (Parsers.contains(i)) {
			count += Parsers[i].length();
		}
	}
	consoleLog("SPOLControllerParserManager: Parsers Count: " + QString::number(count));
	return count;
}