#include "../SPDFTerminal.h"

/*
SPDFParserResult
*/

def_copy SPDFParserResult::SPDFParserResult(const SPDFParserResult& obj) {
	MethodName = obj.MethodName;
	Parameters = obj.Parameters;
	NoWait = obj.NoWait;
}

def_move SPDFParserResult::SPDFParserResult(SPDFParserResult&& obj) {
	MethodName = obj.MethodName;
	Parameters = obj.Parameters;
	NoWait = obj.NoWait;
}

SPDFParserResult& SPDFParserResult::operator=(const SPDFParserResult& obj) {
	MethodName = obj.MethodName;
	Parameters = obj.Parameters;
	NoWait = obj.NoWait;
	return *this;
}

void SPDFAbstractTerminal::controllerFinishedListener(){
	ControllerWaitCount--;
	if (ControllerWaitCount == 0) { controllerHandled(); }
}

void SPDFAbstractTerminal::privateOnControllers(SPDFParserResultList* ParserList, SPDF::SPOLExecutionMode mode) {
	ControllerWaitCount = 0;
	for (auto& i : *ParserList) {
		if (!i.NoWait) { ControllerWaitCount++; }
	}
	onControllers(ParserList, mode);
	if (ControllerWaitCount == 0) { controllerHandled(); }
	delete ParserList;
}