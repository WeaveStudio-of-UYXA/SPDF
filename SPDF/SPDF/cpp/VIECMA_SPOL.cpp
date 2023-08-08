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

void VIECMA_SPOL::preloadStory(QJSValue spolLinesRaw) {
	QStringList spolLines = spolLinesRaw.toVariant().toStringList();
	consoleLog("SPOL preloading story...");
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