#pragma once
#include <VICore>
#include "SPDFPackageMeta.h"
namespace SPDFNamespace {
	enum class SPOLExecutionMode {
		Preload,
		RunTime
	};
}
typedef QMap<QString, QVariant> SPDFParaMap;
class SPDFPublicAPI SPDFParserResult
{
	_Public QString MethodName = "Unknown";
	_Public SPDFParaMap Parameters;
	_Public SPDFNamespace::SPOLExecutionMode ExecutionMode = SPDFNamespace::SPOLExecutionMode::RunTime;
	_Public bool NoWait = false;
	_Public def_init SPDFParserResult() {}
	_Public def_copy SPDFParserResult(const SPDFParserResult& obj);
	_Public def_move SPDFParserResult(SPDFParserResult&& obj);
	_Public SPDFParserResult& operator=(const SPDFParserResult& obj);
};
typedef QVector<SPDFParserResult> SPDFParserResultList;
typedef SPDFParserResult SPDFBottomInterface;
typedef SPDFParserResultList SPDFBottomInterfaceList;
/*
* Abstract Terminal
* 如果用户要从SPDF获得控制数据，必须·继承此类并实现其虚函数
*/

class SPDFHost;
class SPDFPublicAPI SPDFAbstractTerminal :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	VI_PrivateProperty(unsigned long long, ControllerWaitCount);
	_Public SPDFHost* Host;
	_Signal void controllerHandled();
	_Public def_init SPDFAbstractTerminal(VISuper* parent = VI_NULLPTR):VIObject(parent) {}
	_Slot virtual void controllerFinishedListener() final;
	_Slot virtual void preControllers(SPDFParserResultList* ParserList) final;
	_Slot virtual void onControllers(SPDFParserResultList* ParserList) PureVirtual;
};