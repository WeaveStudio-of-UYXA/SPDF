#pragma once
#include <VICore>
#include "SPDFPackageMeta.h"
namespace SPDF {
	enum class SPOLExecutionMode {
		SyntaxCheck,
		Preload,
		RunTime,
		StepDebug,
		Render
	};
}

typedef QMap<QString, QVariant> SPDFParaMap;
class SPDFPublicAPI SPDFControllerData
{
	_Public QString MethodName = "Unknown";
	_Public SPDFParaMap Parameters;
	_Public bool NoWait = false;
	_Public def_init SPDFControllerData() {}
	_Public def_copy SPDFControllerData(const SPDFControllerData& obj);
	_Public def_move SPDFControllerData(SPDFControllerData&& obj);
	_Public SPDFControllerData& operator=(const SPDFControllerData& obj);
};
typedef QVector<SPDFControllerData> SPDFControllerDataList;
typedef SPDFControllerData SPDFBottomInterface;
typedef SPDFControllerDataList SPDFBottomInterfaceList;
/*
* Abstract Stage
* 如果用户要从SPDF获得控制数据，必须·继承此类并实现其虚函数
*/

class SPDFHost;
class SPDFPublicAPI SPDFAbstractStage :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	VI_PrivateProperty(unsigned long long, ControllerWaitCount);
	VI_PrivateProperty(QVariant, ReturnRegister);
	_Public SPDFHost* Host;
	_Signal void controllerHandled();
	_Public def_init SPDFAbstractStage(VISuper* parent = VI_NULLPTR):VIObject(parent) {}
	_Slot virtual void controllerFinishedListener() final;
	_Public void saveReturn(const QVariant& value);
	_Slot virtual void privateOnControllers(SPDFControllerDataList* ParserList, SPDF::SPOLExecutionMode mode) final;
	_Slot virtual void onControllers(SPDFControllerDataList* ParserList, SPDF::SPOLExecutionMode mode) PureVirtual;
	_Slot virtual void onSPOLDocumentChanged(const QString& metaName, SPDF::SPOLExecutionMode mode) PureVirtual;
	_Slot virtual void onSceneFinished(SPDF::SPOLExecutionMode mode) PureVirtual;
};