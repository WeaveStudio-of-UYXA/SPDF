/*
* 此文件含有您的项目的包类的定义，您可以将包类的构造函数当做您的项目的入口函数。
*/
#pragma once
#include "SPDFPackageMeta.h"
#include "SPDFHost.h"
namespace SPDF {
	class SPDFPublicAPI Package :public VIPackage, VITranslatableObject
	{
		Q_OBJECT;
		VI_OBJECT;
		VI_HAS_INHERIT(VIPackage);
		VI_Singleton(Package);
		_Private SPDFHost* Host;
		_Public def_init Package();
		_Public virtual void onTranslating() override HalfVirtual;
		_Public void attachNewHostToTerminal(SPDFAbstractStage* terminal);
	};
}