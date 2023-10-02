#pragma once
#include <VICore>
#include "private/SPDFCompileMacro.h"
class SPDFScripts;
class SPDFPublicAPI VIECMA_SPDF :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	_Public SPDFScripts* Scripts;
	_Public def_init VIECMA_SPDF();
	_Slot void loadStory(QJSValue spolLinesRaw);
	_Slot void loadStoryWithMeta(QJSValue metaName);
	_Slot void preloadStory(QJSValue metaName, QJSValue spolLinesRaw);
	_Slot void preloadSPOLFile(QJSValue spolFilePath);
	_Slot void installParser(QJSValue parser);
	_Slot void installStandardParser(QJSValue parser);
	_Slot void print(QJSValue str);
	_Slot void render(QJSValue spolLinesRaw);
	_Slot void renderAll();
};