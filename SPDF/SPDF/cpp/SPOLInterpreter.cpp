#include "../SPOLInterpreter.h"

def_init SPOLInterpreter::SPOLInterpreter(SPDFAbstractTerminal* terminal, QMutex* mutex, QWaitCondition* waitCondition) {
	Terminal = terminal;
	ThreadMutex = mutex;
	ThreadWaitCondition = waitCondition;
	connect(this, &SPOLInterpreter::onControllers, Terminal, &SPDFAbstractTerminal::privateOnControllers);
	connect(this, &SPOLInterpreter::spolDocumentChanged, Terminal, &SPDFAbstractTerminal::onSPOLDocumentChanged);
	connect(this, &SPOLInterpreter::sceneFinished, Terminal, &SPDFAbstractTerminal::onSceneFinished);
}

def_del SPOLInterpreter::~SPOLInterpreter() {
	disconnect(this, &SPOLInterpreter::onControllers, Terminal, &SPDFAbstractTerminal::privateOnControllers);
	disconnect(this, &SPOLInterpreter::spolDocumentChanged, Terminal, &SPDFAbstractTerminal::onSPOLDocumentChanged);
	disconnect(this, &SPOLInterpreter::sceneFinished, Terminal, &SPDFAbstractTerminal::onSceneFinished);
}

void SPOLInterpreter::addParser(SPDFAbstractControllerParser* parser) {
	if (!Parsers.contains(parser->getControllerFlag())) {
		parser->Context = &SPOLDocumentContext;
		Parsers.insert(parser->getControllerFlag(), parser);
		parser->Interpreter = this;
	}
	else {
		return;
	}
}

void SPOLInterpreter::addSPOL(const QString& metaName, const QStringList& spol) {
	SPOLDocumentMap.insert(metaName, spol);
}

void SPOLInterpreter::wait() {
	ThreadWaitCondition->wait(ThreadMutex);
}

void SPOLInterpreter::executeSPOL(SPDF::SPOLExecutionMode mode, const QStringList& spol) {
	SPOLDocument = spol;
	executeSPOL(mode);
}

unsigned long long SPOLInterpreter::getExecuteLineIndex() {
	return CurrentLine - SPOLDocument.begin();
}

void SPOLInterpreter::changeExecuteLine(unsigned int index) {
	lineChanged = true;
	CurrentLine = SPOLDocument.begin() + index;
}

void SPOLInterpreter::setVariable(const QString& name, const QVariant& value) {
	Variables.insert(name, value);
}

QVariant SPOLInterpreter::getVariable(const QString& name) {
	return Variables[name];
}

QString SPOLInterpreter::getSPOLWithIndex(unsigned int index) {
	return SPOLDocument[index];
}
void SPOLInterpreter::executeSPOL(SPDF::SPOLExecutionMode mode, const QString& spol ) {
	if (spol != "") { SPOLDocument = spol.split("\n"); }
	emit spolDocumentChanged(SPOLDocument, mode);
	wait();
	SceneFinished = false;
	QStringList ControllersName = Parsers.keys();
	for (CurrentLine = SPOLDocument.begin(); CurrentLine != SPOLDocument.end();) {
		if (*CurrentLine == "") { CurrentLine++; continue; }
		if (SceneFinished) {
			break;
		}
		consoleLog("Parsing: " + *CurrentLine);
		for (auto controllerName = ControllersName.begin(); controllerName != ControllersName.end(); controllerName++) {
			if ((*CurrentLine).startsWith(*controllerName)) {
				SPDFAbstractControllerParser* par = Parsers[*controllerName];
				par->Parameters.clear();
				bool success = par->parseLine(*CurrentLine, mode);
				//可能有线程安全问题，因为抽象控制器解析器是在VIECMAScript的线程中运行的
				//但是这里的onControllers目标应该是在主线程中运行的
				//主要目前从逻辑上来说，Parameters在解析之后VIECMAScript中应该没人会动它
				//然后紧接着就是主线程拿去用，用完之后就会被清空，所以暂时不管了
				if (!success) { continue; }//以后可以整个内部异常机制
				else {
					SPDFParserResultList* list = new SPDFParserResultList(*(par->getParameters()));
					if (list->isEmpty()) { break; }
					else {
						emit onControllers(list, mode);
						for (auto i = par->Parameters.begin(); i != par->Parameters.end(); i++) {
							if (!(*i).NoWait) { wait(); }
							break;
						}
						break;
					}
				}
			}
		}
		if (!lineChanged) {
			CurrentLine++;
		}
		else {
			lineChanged = false;
		}
	}
	SceneFinished = true;
	emit sceneFinished(mode);
	wait();
}
