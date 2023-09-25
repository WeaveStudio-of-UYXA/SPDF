#include "../SPOLInterpreter.h"

def_init SPOLInterpreter::SPOLInterpreter(SPDFAbstractTerminal* terminal, QMutex* mutex, QWaitCondition* waitCondition) {
	Terminal = terminal;
	ThreadMutex = mutex;
	ThreadWaitCondition = waitCondition;
	connect(this, &SPOLInterpreter::onControllers, Terminal, &SPDFAbstractTerminal::privateOnControllers);
	connect(this, &SPOLInterpreter::spolDocumentChanged, Terminal, &SPDFAbstractTerminal::onSPOLDocumentChanged);
	connect(this, &SPOLInterpreter::sceneFinished, Terminal, &SPDFAbstractTerminal::onSceneFinished);
	SPOLSegment_SWITCH* sw = new SPOLSegment_SWITCH();
	sw->SegmentName = "SWITCH";
	SegmentParserList.append(sw);
	sw->Interpreter = this;
}

def_del SPOLInterpreter::~SPOLInterpreter() {
	disconnect(this, &SPOLInterpreter::onControllers, Terminal, &SPDFAbstractTerminal::privateOnControllers);
	disconnect(this, &SPOLInterpreter::spolDocumentChanged, Terminal, &SPDFAbstractTerminal::onSPOLDocumentChanged);
	disconnect(this, &SPOLInterpreter::sceneFinished, Terminal, &SPDFAbstractTerminal::onSceneFinished);
	while (!SegmentStack.isEmpty()) {
		SegmentStack.top()->deleteLater();
		SegmentStack.pop();
	}
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

QString SPOLInterpreter::getSPOLWithIndex(unsigned int index) {
	return SPOLDocument[index];
}

quint32 SPOLInterpreter::getCurrentSPOLDocumentLength() {
	return SPOLDocument.length();
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

void SPOLInterpreter::executeSPOL(SPDF::SPOLExecutionMode mode, const QStringList& spol) {
	SPOLDocument = spol;
	emit spolDocumentChanged(SPOLDocument, mode);
	wait();
	SegmentStack.clear();
	SceneFinished = false;
	SPOLSegment_Global* global = new SPOLSegment_Global();
	SegmentStack.push(global);
	CurrentLine = SPOLDocument.begin();
	while (CurrentLine != SPOLDocument.end()) {
		if (SceneFinished) {
			break;
		}
		qint32 indentChar = checkIndent(CurrentLine);
		if (lineChanged) {
			lineChanged = false;
			continue;
		}
		QString line = CurrentLine->right(CurrentLine->length() - indentChar);
		if (line.isEmpty()) { CurrentLine++; continue; }
		executeSPOLSingleLine(line, mode);
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

qint32 SPOLInterpreter::checkIndent(QStringList::iterator& line) {
	qint32 indent = 0;
	qint32 indentCount = 0;
	for (auto c : *line) {
		if (c == '\t') {
			indentCount += 4;
			indent++;
		}
		else if (c == ' ') {
			indentCount++;
			indent++;
		}
		else {
			break;
		}
	}
	CurrentIndent = indentCount;
	qint32 LastIndent;
	if (SegmentStack.top()->SegmentIndentStack.isEmpty()) {
		LastIndent = SegmentStack.top()->FirstIndentStack.top();
	}
	else {
		LastIndent = SegmentStack.top()->SegmentIndentStack.top();
	}
	if (LastIndent == indentCount) {
		return indent;
	}
	else if (LastIndent > indentCount) {
		while (SegmentStack.top()->SegmentIndentStack.top() >= indentCount) {
			bool exit = SegmentStack.top()->onIndentMinus();
			if (!exit) {
				break;
			}
			else {
				popSegment();
			}
		}
	}
	else {
		if (allowIndentAdd) {
			SegmentStack.top()->SegmentIndentStack.push(indentCount);
		}
		else {
			throw "Unexpected Indent add. Syntax Error.";
		}
		allowIndentAdd = false;
	}
	return indent;
}

qint32 SPOLInterpreter::indentCount(const QString& line) {
	qint32 indentCount = 0;
	for (auto c : line) {
		if (c == '\t') {
			indentCount += 4;
		}
		else if (c == ' ') {
			indentCount++;
		}
		else {
			break;
		}
	}
	return indentCount;
}

qint32 SPOLInterpreter::indentCharCount(const QString& line) {
	qint32 indentCount = 0;
	for (auto c : line) {
		if (c == '\t' || c== ' ') {
			indentCount ++;
		}
		else {
			break;
		}
	}
	return indentCount;
}

void SPOLInterpreter::executeSPOL(SPDF::SPOLExecutionMode mode, const QString& metaName ) {
	if (metaName == "") { return; }
	if (SPOLDocumentMap.contains(metaName)) {
		executeSPOL(mode, SPOLDocumentMap[metaName]);
	}
}

void SPOLInterpreter::executeSPOLSingleLine(const QString& line, SPDF::SPOLExecutionMode mode) {
	consoleLog("Parsing: " + line);
	for (auto i : SegmentParserList) {
		if (line.startsWith(i->SegmentName)) {
			SegmentStack.push(i);
			i->FirstIndentStack.push(CurrentIndent);
			i->onParseLine(line, mode);
			allowIndentAdd = true;
			return;
		}
	}
	if (Parsers.isEmpty()) { return; }
	for (auto controllerName = Parsers.keys().begin(); controllerName != Parsers.keys().end(); controllerName++) {
		if (line.startsWith(*controllerName)) {
			SPDFAbstractControllerParser* par = Parsers[*controllerName];
			par->Parameters.clear();
			bool success = par->parseLine(line, mode);
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
}

void SPOLInterpreter::popSegment() {
	SegmentStack.top()->FirstIndentStack.pop();
	SegmentStack.top()->SegmentIndentStack.pop();
	SegmentStack.pop();
}
void SPOLInterpreter::wait() {
	ThreadWaitCondition->wait(ThreadMutex);
}
