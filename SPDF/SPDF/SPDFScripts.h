#pragma once
#include "SPOLInterpreter.h"
#include "VIECMA_SPOL.h"
#include "SPSReader.h"

class SPDFScripts :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	_Signal void starting();
	_Signal void storyFilePrepared();
	_Signal void interpreterPrepared();
	_Signal void finished();
	friend class VIECMA_SPOL;
	_Protected VIECMAScripts* ScriptsEngine;
	_Protected SPOLInterpreter* Interpreter = VI_NULLPTR;
	_Private QMutex* Mutex;
	_Private QWaitCondition* WaitCondition;
	_Private QList<SPDFAbstractControllerParser*> Parsers;
	_Private VIECMA_SPOL* ESSPOL;
	_Private SPDFAbstractTerminal* Terminal;
	_Public def_init SPDFScripts(SPDFAbstractTerminal* ter, VISuper* parent = VI_NULLPTR);
	_Public void addParser(SPDFAbstractControllerParser* parser);
	_Public void exec(const QString& path, const QString& entry = "main");
	_Public void awake();
	_Slot void onThreadFinished();
};