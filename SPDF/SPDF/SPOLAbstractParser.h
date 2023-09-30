#pragma once
#include "SPDFTerminal.h"

class SPOLInterpreter;
class SPDFPublicAPI SPOLAbstractControllerParser :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	friend class SPOLInterpreter;
	VI_ProtectedProperty(QString, ControllerFlag);
	VI_ProtectedFlag(FunctionLike);
	_Protected SPOLInterpreter* Interpreter;
	_Protected SPDFControllerDataList Parameters;
	_Private SPDF::SPOLExecutionMode ExecutionMode;
	_Public def_init SPOLAbstractControllerParser();
	_Private virtual bool parseLine(const QString& line, SPDF::SPOLExecutionMode mode) final;
	_Protected void recordParserResult(const SPDFControllerData& result);
	_Public virtual bool onParseLine(const QString& line, SPDF::SPOLExecutionMode mode) PureVirtual;
	_Public unsigned long long getExecuteLineIndex();
	_Public void changeExecuteLine(unsigned int index);
	_Public void setVariable(const QString& name, const QVariant& value);
	_Public QVariant getVariable(const QString& name);
	_Public QString getSPOLWithIndex(unsigned int index);
	_Protected virtual SPDFControllerDataList* getParameters();
};

typedef SPOLAbstractControllerParser SPDFTopInterface;

class SPDFPublicAPI SPOLVIESParserContainer :public SPOLAbstractControllerParser
{
	Q_OBJECT;
	VI_OBJECT;
	_Public QJSValue VIES_SPDFParser;
	_Public QJSValue VIES_onParseLine;
	_Public QJSEngine* Engine;
	_Public def_init SPOLVIESParserContainer(QJSValue parser);
	_Public virtual bool onParseLine(const QString& line, SPDF::SPOLExecutionMode mode);
	_Public virtual SPDFControllerDataList* getParameters() override;
};

class SPDFPublicAPI SPOLControllerParserManager :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	_Public QMap<int, QList<SPOLAbstractControllerParser*>> Parsers;
	VI_PrivateProperty(int, MaxControllerFlagLength);
	VI_PrivateProperty(int, ParsersCount);
	_Public def_init SPOLControllerParserManager(VISuper* super);
	_Public void addSPOLParser(SPOLAbstractControllerParser* parser);
	_Public SPOLAbstractControllerParser* identifyFlag(const QString& spol);
	_Private int deriveParsersCount();
};