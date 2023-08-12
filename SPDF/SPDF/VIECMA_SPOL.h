#pragma once
#include <VICore>
#include "private/SPDFCompileMacro.h"
class SPDFScripts;
class SPDFPublicAPI VIECMA_SPOL :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	_Public SPDFScripts* Scripts;
	_Public def_init VIECMA_SPOL();
	_Slot void loadStory(QJSValue spolLinesRaw);
	_Slot void preloadStory(QJSValue metaName, QJSValue spolLinesRaw);
	_Slot void installParser(QJSValue parser);
	_Slot void print(QJSValue str);
	_Slot void render(QJSValue spolLinesRaw);
	_Slot void renderAll();
};