#pragma once
#include "SPDFTerminal.h"

class SPOLInterpreter;
class SPDFAbstractControllerParser :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	friend class SPOLInterpreter;
	VI_ProtectedProperty(QString, ControllerName);
	VI_ProtectedFlag(FunctionLike);
	VI_ProtectedProperty(QColor, ControllerColor);
	VI_ProtectedFlag(DefaultColor);
	_Protected SPOLInterpreter* Interpreter;
	_Protected SPDFParserResultList Parameters;
	_Public QMap<QString, QString>* Context;
	_Private SPDFNamespace::SPOLExecutionMode ExecutionMode;
	_Public def_init SPDFAbstractControllerParser();
	_Private virtual bool parseLine(const QString& line, SPDFNamespace::SPOLExecutionMode mode) final;
	_Protected void recordParserResult(const SPDFParserResult& result);
	_Public virtual bool onParseLine(const QString& line, SPDFNamespace::SPOLExecutionMode mode) PureVirtual;
	_Public unsigned long long getExecuteLineIndex();
	_Public void changeExecuteLine(unsigned int index);
	_Public void setVariable(const QString& name, const QVariant& value);
	_Public QVariant getVariable(const QString& name);
	_Public QString getSPOLWithIndex(unsigned int index);
	_Protected virtual SPDFParserResultList* getParameters();
};

typedef SPDFAbstractControllerParser SPDFTopInterface;

class SPDFVIESParserContainer :public SPDFAbstractControllerParser
{
	Q_OBJECT;
	VI_OBJECT;
	_Public QJSValue VIES_SPDFParser;
	_Public QJSValue VIES_onParseLine;
	_Public QJSEngine* Engine;
	_Public def_init SPDFVIESParserContainer(QJSValue parser);
	_Public virtual bool onParseLine(const QString& line, SPDFNamespace::SPOLExecutionMode mode);
	_Public virtual SPDFParserResultList* getParameters() override;
};