#include "../SPDFHost.h"

def_init SPDFHost::SPDFHost(SPDFAbstractTerminal* ter, VISuper* parent) :VIObject(parent) {
	Terminal = ter;
	Scripts = new SPDFScripts(Terminal, this);
}
void SPDFHost::installParser(SPDFAbstractControllerParser* p) {
	Scripts->addParser(p);
}
void SPDFHost::exec(QString filePath) {
	Scripts->exec(filePath);
}