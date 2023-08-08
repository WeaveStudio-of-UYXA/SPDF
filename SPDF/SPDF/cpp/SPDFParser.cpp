#include "../SPDFParser.h"
#include "../SPOLInterpreter.h"

def_init SPDFAbstractControllerParser::SPDFAbstractControllerParser() {
	Interpreter = nullptr;
	ControllerFlag = "Unknown";
	FunctionLike = false;
	ControllerColor = QColor(0, 0, 0);
	DefaultColor = true;
}

bool SPDFAbstractControllerParser::parseLine(const QString& line, SPDF::SPOLExecutionMode mode) {
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

void SPDFAbstractControllerParser::recordParserResult(const SPDFParserResult& result) {
	Parameters.append(result);
}

unsigned long long SPDFAbstractControllerParser::getExecuteLineIndex() {
	return Interpreter->getExecuteLineIndex();
}

void SPDFAbstractControllerParser::changeExecuteLine(unsigned int index) {
	Interpreter->changeExecuteLine(index);
}

void SPDFAbstractControllerParser::setVariable(const QString& name, const QVariant& value) {
	Interpreter->setVariable(name, value);
}

QVariant SPDFAbstractControllerParser::getVariable(const QString& name) {
	return Interpreter->getVariable(name);
}

QString SPDFAbstractControllerParser::getSPOLWithIndex(unsigned int index) {
	return Interpreter->getSPOLWithIndex(index);
}

SPDFParserResultList* SPDFAbstractControllerParser::getParameters() {
	return &Parameters;
}

QStringList SPDFAbstractControllerParser::scientificSplit(const QString& str, const QChar& ch) {
	QStringList result;
	QString temp = "";
	bool backslash = false;
	for (auto i = 0; i < str.length(); i++) {
		if (str[i] == ch && !backslash) {
			result.append(temp);
			temp = "";
		}
		else {
			if (str[i] == '\\') {
				backslash = true;
			}
			else {
				backslash = false;
			}
			temp += str[i];
		}
	}
	result.append(temp);
	return result;
}
def_init  SPDFVIESParserContainer::SPDFVIESParserContainer(QJSValue parser) {
	Engine = parser.engine();
	VIES_SPDFParser = parser;
	VIES_onParseLine = parser.property("onParseLine");
	ControllerFlag = parser.property("ControllerFlag").toString();
	FunctionLike = parser.property("FunctionLike").toBool();
}

bool SPDFVIESParserContainer::onParseLine(const QString& line, SPDF::SPOLExecutionMode mode) {
	QJSValueList args;
	args << line;
	args << (int)mode;
	QJSValue ParaMapList = Engine->newArray();
	VIES_SPDFParser.setProperty("Parameters", ParaMapList);
	QJSValue result = VIES_onParseLine.callWithInstance(VIES_SPDFParser, args);
	return result.toBool();
}

SPDFParserResultList* SPDFVIESParserContainer::getParameters() {
	QJSValue ParaMapList = VIES_SPDFParser.property("Parameters");
	int length = ParaMapList.property("length").toInt();
	for (auto i = 0; i < length; i++) {
		QJSValue VIESObj = ParaMapList.property(i);
		SPDFParserResult obj;
		obj.MethodName = VIESObj.property("MethodName").toString();
		obj.NoWait = VIESObj.property("NoWait").toBool();
		obj.Parameters = VIESObj.property("Parameters").toVariant().toMap();
		Parameters.append(obj);
	}
	return &Parameters;
}