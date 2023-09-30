#include "../SPOLInterpreter.h"

def_init SPOLInterpreter::SPOLInterpreter(SPDFAbstractStage* terminal, QMutex* mutex, QWaitCondition* waitCondition) {
	setObjectName(SPDF::PackageMeta::getInstance()->getPackageVersion());
	DocumentManager = new SPOLDocumentManager(this);
	ParserManager = new SPOLControllerParserManager(this);
	Terminal = terminal;
	ThreadMutex = mutex;
	ThreadWaitCondition = waitCondition;
	connect(this, &SPOLInterpreter::onControllers, Terminal, &SPDFAbstractStage::privateOnControllers);
	connect(this, &SPOLInterpreter::spolDocumentChanged, Terminal, &SPDFAbstractStage::onSPOLDocumentChanged);
	connect(this, &SPOLInterpreter::sceneFinished, Terminal, &SPDFAbstractStage::onSceneFinished);
	SPOLSegment_SWITCH* sw = new SPOLSegment_SWITCH();
	sw->SegmentName = "SWITCH";
	SegmentParserList.append(sw);
	sw->Interpreter = this;
}

def_del SPOLInterpreter::~SPOLInterpreter() {
	disconnect(this, &SPOLInterpreter::onControllers, Terminal, &SPDFAbstractStage::privateOnControllers);
	disconnect(this, &SPOLInterpreter::spolDocumentChanged, Terminal, &SPDFAbstractStage::onSPOLDocumentChanged);
	disconnect(this, &SPOLInterpreter::sceneFinished, Terminal, &SPDFAbstractStage::onSceneFinished);
	while (!SegmentStack.isEmpty()) {
		SegmentStack.top()->deleteLater();
		SegmentStack.pop();
	}
}

void SPOLInterpreter::addParser(SPOLAbstractControllerParser* parser) {
	ParserManager->addSPOLParser(parser);
}

void SPOLInterpreter::addSPOL(const QString& metaName, const QStringList& spol) {
	DocumentManager->addSPOLDocument(metaName, spol);
}

QString SPOLInterpreter::getSPOLWithIndex(unsigned int index) {
	return DocumentManager->getLine(index);
}

quint32 SPOLInterpreter::getCurrentSPOLDocumentLength() {
	return DocumentManager->getCurrentDocumentLength();
}
unsigned long long SPOLInterpreter::getExecuteLineIndex() {
	return DocumentManager->getCurrentLineIndex();
}

void SPOLInterpreter::changeExecuteLine(unsigned int index) {
	DocumentManager->changeCurrentLineIndex(index);
}

void SPOLInterpreter::setVariable(const QString& name, const QVariant& value) {
	FuncStack.top()->ParamStack.top()[name] = value;
}

QVariant SPOLInterpreter::getVariable(const QString& name) {
	if (FuncStack.top()->ParamStack.top().contains(name)) {
		return FuncStack.top()->ParamStack.top()[name];
	}
	else {
		return QVariant();
	}
}

void SPOLInterpreter::executeSPOL(SPDF::SPOLExecutionMode mode, const QStringList& spol) {
	DocumentManager->changeCurrentDocument(spol);
	executeSPOL(mode);
}

void SPOLInterpreter::executeSPOL(SPDF::SPOLExecutionMode mode, const QString& metaName) {
	if (metaName == "") { return; }
	if (DocumentManager->hasDocument(metaName)) {
		DocumentManager->changeCurrentDocument(metaName);
		executeSPOL(mode);
	}
}

void SPOLInterpreter::executeSPOL(SPDF::SPOLExecutionMode mode) {
	SegmentStack.clear();
	SceneFinished = false;
	SPOLSegment_FUNC* global = new SPOLSegment_FUNC();
	SegmentStack.push(global);
	FuncStack.push(global);
	SceneFinished = false;
	emit spolDocumentChanged(DocumentManager->getCurrentDocumentMetaName(), mode);
	wait();
	while (!DocumentManager->iterateEnd()) {
		if (SceneFinished) {
			break;
		}
		QString CurrentLine = DocumentManager->iterateNextLine();
		qint32 indentChar = checkIndent(CurrentLine);
		if (DocumentManager->currentLineChanged()) {
			continue;
		}
		QString line = CurrentLine.right(CurrentLine.length() - indentChar);
		if (line.isEmpty()) { continue; }
		executeSPOLSingleLine(line, mode);
	}
	SceneFinished = true;
	emit sceneFinished(mode);
	wait();
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
	if (ParserManager->getParsersCount() == 0) { return; }
	SPOLAbstractControllerParser* parser = ParserManager->identifyFlag(line);
	if (parser == nullptr) { 
		consoleLog("No parser identified. Skip.");
		return; 
	} //这也需要一个兜底控制器
	parser->Parameters.clear();
	bool success = parser->parseLine(line, mode);
	//可能有线程安全问题，因为抽象控制器解析器是在VIECMAScript的线程中运行的
	//但是这里的onControllers目标应该是在主线程中运行的
	//主要目前从逻辑上来说，Parameters在解析之后VIECMAScript中应该没人会动它
	//然后紧接着就是主线程拿去用，用完之后就会被清空，所以暂时不管了
	if (!success) {
		//需要补充兜底控制器
	}
	else {
		SPDFControllerDataList* list = new SPDFControllerDataList(*(parser->getParameters()));
		if (!list->isEmpty()) {
			bool Wait = false;
			for (auto i = parser->Parameters.begin(); i != parser->Parameters.end(); i++) {
				if (!(*i).NoWait) { Wait = true; }
				break;
			}
			emit onControllers(list, mode);
			if (Wait) { wait(); }
		}
	}
}

qint32 SPOLInterpreter::checkIndent(const QString& line) {
	qint32 indent = 0;
	qint32 indentCount = 0;
	for (auto c : line) {
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
		while (SegmentStack.top()->SegmentIndentStack.top() > indentCount) {
			if (SegmentStack.length() == 1) {
				break;
			}
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
		if (c == '\t' || c == ' ') {
			indentCount++;
		}
		else {
			break;
		}
	}
	return indentCount;
}

QString SPOLInterpreter::removeIndent(QString line) {
	while(line.startsWith('\t') || line.startsWith(' ')) {
		line.remove(0, 1);
	}
	return line;
}
void SPOLInterpreter::popSegment() {
	SegmentStack.top()->FirstIndentStack.pop();
	SegmentStack.top()->SegmentIndentStack.pop();
	SegmentStack.pop();
}
void SPOLInterpreter::wait() {
	ThreadWaitCondition->wait(ThreadMutex);
}
