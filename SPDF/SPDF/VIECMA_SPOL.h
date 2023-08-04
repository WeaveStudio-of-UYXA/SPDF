#pragma once
#include <VICore>

class SPDFScripts;
class VIECMA_SPOL :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	_Public SPDFScripts* Scripts;
	_Public def_init VIECMA_SPOL();
	_Slot void loadStory(QJSValue spolLinesRaw);
	_Slot void preloadStory(QJSValue spolLinesRaw);
	_Slot void installParser(QJSValue parser);
};