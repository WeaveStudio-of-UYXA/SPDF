#include "../SPOLAbstractSegment.h"
#include "../SPOLInterpreter.h"

void SPOLSegment_SWITCH::onParseLine(const QString& line, SPDF::SPOLExecutionMode mode) {
	QString exp = line.mid(7, line.length() - 7); // 7 = switch + space
	int p = 1;
	bool getACASE = false;
	qint32 caseIndent = 0;
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
				if (caseExp == exp) {
					Interpreter->changeExecuteLine(Interpreter->getExecuteLineIndex() + p + 1);
					return;
				}
			}
			p++;
		}
	}
}

bool SPOLSegment_SWITCH::onIndentMinus() {
	int p = 0;
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