#include "../SPOLInterpreter.h"
#include "../VIECMA_SPOL.h"
#include "../SPDFScripts.h"

def_init VIECMA_SPOL::VIECMA_SPOL() {
	setObjectName("SPOL");
}
void VIECMA_SPOL::loadStory(QJSValue spolLinesRaw) {
	QStringList spolLines = spolLinesRaw.toVariant().toStringList();
	consoleLog("SPOL executing story...");
	Scripts->Interpreter->executeSPOL(SPDF::SPOLExecutionMode::RunTime, spolLines);
	consoleLog("SPOL story executed.");
}

void VIECMA_SPOL::preloadStory(QJSValue metaName, QJSValue spolLinesRaw) {
	QStringList spolLines = spolLinesRaw.toVariant().toStringList();
	consoleLog("SPOL preloading story...");
	Scripts->Interpreter->addSPOL(metaName.toString(), spolLines);
	Scripts->Interpreter->executeSPOL(SPDF::SPOLExecutionMode::Preload, spolLines);
	consoleLog("SPOL story preloaded.");
}
void VIECMA_SPOL::installParser(QJSValue parser) {
	SPDFVIESParserContainer* parserContainer = new SPDFVIESParserContainer(parser);
	Scripts->Interpreter->addParser(parserContainer);
}
void VIECMA_SPOL::print(QJSValue text) {
	qDebug() << text.toString();
}
void VIECMA_SPOL::render(QJSValue spolLinesRaw) {
	QStringList spolLines = spolLinesRaw.toVariant().toStringList();
	consoleLog("SPOL rendering story...");
	Scripts->Interpreter->executeSPOL(SPDF::SPOLExecutionMode::Render, spolLines);
	consoleLog("SPOL story rendered.");
}
void VIECMA_SPOL::renderAll() {
	consoleLog("SPOL rendering all stories...");
	for (auto it = Scripts->Interpreter->SPOLDocumentMap.begin(); it != Scripts->Interpreter->SPOLDocumentMap.end(); it++) {
		if (it.value().size() == 0) { continue; }
		if (it.key().contains("__head__")) { continue; }
		Scripts->Interpreter->setCurrentMetaName(it.key());
		Scripts->Interpreter->executeSPOL(SPDF::SPOLExecutionMode::Render, it.value());
	}
	consoleLog("SPOL all stories rendered.");
}