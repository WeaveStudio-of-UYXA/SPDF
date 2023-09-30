#pragma once
#include "SPOLInterpreter.h"
#include "VIECMA_SPDF.h"
#include "SPSReader.h"

class SPDFPublicAPI SPDFScripts :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	_Signal void starting();
	_Signal void storyFilePrepared();
	_Signal void interpreterPrepared();
	_Signal void finished();
	friend class VIECMA_SPDF;
	_Protected VIECMAScripts* ScriptsEngine;
	_Public SPOLInterpreter* Interpreter = VI_NULLPTR;
	_Private QMutex* Mutex;
	_Private QWaitCondition* WaitCondition;
	_Private QList<SPOLAbstractControllerParser*> Parsers;
	_Private VIECMA_SPDF* ESSPOL;
	_Private SPDFAbstractStage* Terminal;
	_Public def_init SPDFScripts(SPDFAbstractStage* ter, VISuper* parent = VI_NULLPTR);
	_Public void addParser(SPOLAbstractControllerParser* parser);
	_Public void exec(const QString& path, const QString& entry = "main");
	_Public void awake();
	_Slot void onThreadFinished();
};