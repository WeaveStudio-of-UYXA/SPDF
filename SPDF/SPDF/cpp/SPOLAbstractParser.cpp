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

def_init SPOLControllerParserManager::SPOLControllerParserManager(VISuper* super):VIObject(super) {
	MaxControllerFlagLength = 0;
	ParsersCount = 0;
}

void SPOLControllerParserManager::addSPOLParser(SPOLAbstractControllerParser* parser) {
	QString flag = parser->getControllerFlag();
	int length = flag.length();
	if (length > MaxControllerFlagLength) {
		MaxControllerFlagLength = length;
	}
	if (Parsers.contains(length)) {
		if (Parsers[length].contains(parser)) {
			return;
		}
		else {
			Parsers[length].append(parser);
		}
	}
	setParsersCount(deriveParsersCount());
}

SPOLAbstractControllerParser* SPOLControllerParserManager::identifyFlag(const QString& spol) {
	for (auto i = MaxControllerFlagLength; i > 0; i--) {
		if (Parsers.contains(i)) {
			for (auto parser : Parsers[i]) {
				if (spol.startsWith(parser->getControllerFlag())) {
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
	return count;
}