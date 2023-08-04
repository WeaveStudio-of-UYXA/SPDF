﻿#include "../SPDFParser.h"
#include "../SPOLInterpreter.h"

def_init SPDFAbstractControllerParser::SPDFAbstractControllerParser() {
	Interpreter = nullptr;
	ControllerName = "Unknown";
	FunctionLike = false;
	ControllerColor = QColor(0, 0, 0);
	DefaultColor = true;
}

bool SPDFAbstractControllerParser::parseLine(const QString& line, SPDFNamespace::SPOLExecutionMode mode) {
	ExecutionMode = mode;
	int cnLength = ControllerName.length();
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
	for (auto i : Parameters) {
		i.ExecutionMode = ExecutionMode;
	}
	return &Parameters;
}

def_init  SPDFVIESParserContainer::SPDFVIESParserContainer(QJSValue parser) {
	Engine = parser.engine();
	VIES_SPDFParser = parser;
	VIES_onParseLine = parser.property("onParseLine");
	ControllerName = parser.property("ControllerName").toString();
	FunctionLike = parser.property("FunctionLike").toBool();
}

bool SPDFVIESParserContainer::onParseLine(const QString& line, SPDFNamespace::SPOLExecutionMode mode) {
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