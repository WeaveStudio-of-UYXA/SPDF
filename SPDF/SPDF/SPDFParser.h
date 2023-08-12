#pragma once
#include "SPDFTerminal.h"

class SPOLInterpreter;
class SPDFPublicAPI SPDFAbstractControllerParser :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	friend class SPOLInterpreter;
	VI_ProtectedProperty(QString, ControllerFlag);
	VI_ProtectedFlag(FunctionLike);
	VI_ProtectedProperty(QColor, ControllerColor);
	VI_ProtectedFlag(DefaultColor);
	_Protected SPOLInterpreter* Interpreter;
	_Protected SPDFParserResultList Parameters;
	_Public QMap<QString, QString>* Context;
	_Private SPDF::SPOLExecutionMode ExecutionMode;
	_Public def_init SPDFAbstractControllerParser();
	_Private virtual bool parseLine(const QString& line, SPDF::SPOLExecutionMode mode) final;
	_Protected void recordParserResult(const SPDFParserResult& result);
	_Public virtual bool onParseLine(const QString& line, SPDF::SPOLExecutionMode mode) PureVirtual;
	_Public unsigned long long getExecuteLineIndex();
	_Public void changeExecuteLine(unsigned int index);
	_Public void setVariable(const QString& name, const QVariant& value);
	_Public QVariant getVariable(const QString& name);
	_Public QString getSPOLWithIndex(unsigned int index);
	_Protected virtual SPDFParserResultList* getParameters();
	_Public QStringList scientificSplit(const QString& str, const QChar& ch);
};

typedef SPDFAbstractControllerParser SPDFTopInterface;

class SPDFPublicAPI SPDFVIESParserContainer :public SPDFAbstractControllerParser
{
	Q_OBJECT;
	VI_OBJECT;
	_Public QJSValue VIES_SPDFParser;
	_Public QJSValue VIES_onParseLine;
	_Public QJSEngine* Engine;
	_Public def_init SPDFVIESParserContainer(QJSEngine* engine, QJSValue parser);
	_Public virtual bool onParseLine(const QString& line, SPDF::SPOLExecutionMode mode);
	_Public virtual SPDFParserResultList* getParameters() override;
};