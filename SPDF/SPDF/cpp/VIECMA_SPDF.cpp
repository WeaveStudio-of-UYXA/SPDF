#include "../SPOLInterpreter.h"
#include "../VIECMA_SPDF.h"
#include "../SPDFScripts.h"

def_init VIECMA_SPDF::VIECMA_SPDF() {
	setObjectName("SPDF");
}
void VIECMA_SPDF::loadStory(QJSValue spolLinesRaw) {
	QStringList spolLines = spolLinesRaw.toVariant().toStringList();
	consoleLog("SPOL executing story...");
	Scripts->Interpreter->executeSPOL(SPDF::SPOLExecutionMode::RunTime, spolLines);
	consoleLog("SPOL story executed.");
}

void VIECMA_SPDF::preloadStory(QJSValue metaName, QJSValue spolLinesRaw) {
	QStringList spolLines = spolLinesRaw.toVariant().toStringList();
	consoleLog("SPOL preloading story...");
	Scripts->Interpreter->addSPOL(metaName.toString(), spolLines);
	Scripts->Interpreter->executeSPOL(SPDF::SPOLExecutionMode::Preload, spolLines);
	consoleLog("SPOL story preloaded.");
}
void VIECMA_SPDF::installParser(QJSValue parser) {
	SPOLVIESParserContainer* parserContainer = new SPOLVIESParserContainer(parser);
	Scripts->Interpreter->addParser(parserContainer);
}
void VIECMA_SPDF::print(QJSValue text) {
	qDebug() << text.toString();
}
void VIECMA_SPDF::render(QJSValue spolLinesRaw) {
	QStringList spolLines = spolLinesRaw.toVariant().toStringList();
	consoleLog("SPOL rendering story...");
	Scripts->Interpreter->executeSPOL(SPDF::SPOLExecutionMode::Render, spolLines);
	consoleLog("SPOL story rendered.");
}
void VIECMA_SPDF::renderAll() {
	consoleLog("SPOL rendering all stories...");
	QStringList metaNames = Scripts->Interpreter->DocumentManager->getMetaNames();
	for (auto it = metaNames.begin(); it != metaNames.end(); it++) {
		if (it->contains("__head__")) { continue; }
		Scripts->Interpreter->executeSPOL(SPDF::SPOLExecutionMode::Render, *it);
	}
	consoleLog("SPOL all stories rendered.");
}