#include "../SPOLInterpreter.h"
#include "../VIECMA_SPOL.h"
#include "../SPDFScripts.h"

def_init VIECMA_SPOL::VIECMA_SPOL() {
	setObjectName("SPOL");
}
void VIECMA_SPOL::loadStory(QJSValue spolLinesRaw) {
	QStringList spolLines = spolLinesRaw.toVariant().toStringList();
	Scripts->Interpreter->executeSPOL(SPDFNamespace::SPOLExecutionMode::RunTime, spolLines);
}

void VIECMA_SPOL::preloadStory(QJSValue spolLinesRaw) {
	QStringList spolLines = spolLinesRaw.toVariant().toStringList();
	Scripts->Interpreter->executeSPOL(SPDFNamespace::SPOLExecutionMode::Preload, spolLines);
}
void VIECMA_SPOL::installParser(QJSValue parser) {
	SPDFVIESParserContainer* parserContainer = new SPDFVIESParserContainer(parser);
	Scripts->Interpreter->addParser(parserContainer);
}