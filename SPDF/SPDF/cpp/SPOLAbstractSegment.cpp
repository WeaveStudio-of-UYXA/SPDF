#include "../SPOLAbstractSegment.h"
#include "../SPOLInterpreter.h"

bool SPOLSegment_SWITCH::onParseLine(const QString& line, SPDF::SPOLExecutionMode mode) {
	QString exp = Interpreter->getReturnRegister().toString();
	int p = 0;
	bool getACASE = false;
	qint32 caseIndent = 0;
	bool haveCASE = false;
	while (true) {
		if (Interpreter->getExecuteLineIndex() + p >= Interpreter->getCurrentSPOLDocumentLength()) {
			break;
		}
		QString SPOL = Interpreter->getSPOLWithIndex(Interpreter->getExecuteLineIndex() + p);
		qint32 indent = SPOLInterpreter::indentCount(SPOL);
		qint32 indentChar = SPOLInterpreter::indentCharCount(SPOL);
		if (indent <= FirstIndentStack.top()) {
			Interpreter->changeExecuteLine(Interpreter->getExecuteLineIndex() + p);
			SegmentIndentStack.push(indent);
			Interpreter->popSegment();
			return false;
		}
		else {
			if (getACASE) {
				if (indent != caseIndent) {
					p++;
					continue;
				}
			}
			SPOL = SPOL.right(SPOL.length() - indentChar);
			if (SPOL.startsWith("CASE")) {
				if (!getACASE) {
					getACASE = true;
					caseIndent = indent;
				}
				QString caseExp = SPOL.mid(5, SPOL.length() - 5);
				consoleLog("caseExp: " + caseExp);
				consoleLog("exp: " + exp);
				if (caseExp == exp) {
					Interpreter->changeExecuteLine(Interpreter->getExecuteLineIndex() + p + 1);
					return true;
				}
			}
			p++;
		}
	}
	Interpreter->changeExecuteLine(Interpreter->getExecuteLineIndex() + p);
	SegmentIndentStack.push(FirstIndentStack.top());
	Interpreter->popSegment();
	return false;
}

bool SPOLSegment_SWITCH::onIndentMinus() {
	int p = -1;
	while (true) {
		if (Interpreter->getExecuteLineIndex() + p >= Interpreter->getCurrentSPOLDocumentLength()) {
			break;
		}
		QString SPOL = Interpreter->getSPOLWithIndex(Interpreter->getExecuteLineIndex() + p);
		qint32 indent = SPOLInterpreter::indentCount(SPOL);
		qint32 indentChar = SPOLInterpreter::indentCharCount(SPOL);
		if (indent <= FirstIndentStack.top()) {
			Interpreter->changeExecuteLine(Interpreter->getExecuteLineIndex() + p);
			break;
		}
		else {
			p++;
		}
	}
	return true;
}