#include "../SPDFScripts.h"

def_init SPDFScripts::SPDFScripts(SPDFAbstractTerminal* ter, VISuper* parent) :VIObject(parent) {
	Terminal = ter;
	ScriptsEngine = new VIECMAScripts();
	Mutex = ScriptsEngine->getThreadMutex();
	WaitCondition = ScriptsEngine->getThreadWaitCondition();
	connect(ScriptsEngine, &VIECMAScripts::finished, this, &SPDFScripts::onThreadFinished);
	connect(Terminal, &SPDFAbstractTerminal::controllerHandled, this, &SPDFScripts::awake);
	ESSPOL = new VIECMA_SPOL();
	ESSPOL->Scripts = this;
	ScriptsEngine->importVIObject(ESSPOL);
}

void SPDFScripts::addParser(SPDFAbstractControllerParser* parser) {
	Parsers.append(parser);
}

void SPDFScripts::exec(const QString& path, const QString& entry) {
	emit starting();
	if (ESSPOL == VI_NULLPTR) {
		ESSPOL = new VIECMA_SPOL();
		ESSPOL->Scripts = this;
		ScriptsEngine->importVIObject(ESSPOL);
	}
	consoleLog("Preparing to convert VIS to standard ES6...");
	SPSReader::spawnAllStoryFile(path);
	consoleLog("VIS converted to standard ES6.");
	emit storyFilePrepared();
	consoleLog("Initialize SPOL interpreter");
	if (Interpreter != VI_NULLPTR) {
		Interpreter->deleteLater();
	}
	Interpreter = new SPOLInterpreter(Terminal, Mutex, WaitCondition);
	for (auto parser : Parsers) {
		Interpreter->addParser(parser);
	}
	Interpreter->moveToThread(ScriptsEngine->getThread());
	consoleLog("The SPOL interpreter has been initialized and merged into the VIECMA thread");
	emit interpreterPrepared();
	consoleLog("Start VIECMA");
	ScriptsEngine->threadBoot(path, entry);
}

void SPDFScripts::awake() {
	WaitCondition->wakeAll();
}

void SPDFScripts::onThreadFinished() {
	Interpreter->deleteLater();
	Interpreter = VI_NULLPTR;
	emit finished();
}