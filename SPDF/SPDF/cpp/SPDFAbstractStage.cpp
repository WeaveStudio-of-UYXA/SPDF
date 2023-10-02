#include "../SPDFAbstractStage.h"

/*
SPDFControllerData
*/

def_copy SPDFControllerData::SPDFControllerData(const SPDFControllerData& obj) {
	MethodName = obj.MethodName;
	Parameters = obj.Parameters;
	NoWait = obj.NoWait;
}

def_move SPDFControllerData::SPDFControllerData(SPDFControllerData&& obj) {
	MethodName = obj.MethodName;
	Parameters = obj.Parameters;
	NoWait = obj.NoWait;
}

SPDFControllerData& SPDFControllerData::operator=(const SPDFControllerData& obj) {
	MethodName = obj.MethodName;
	Parameters = obj.Parameters;
	NoWait = obj.NoWait;
	return *this;
}

void SPDFAbstractStage::saveReturn(const QVariant& value) {
	ReturnRegister = value;
	controllerHandled();
}
void SPDFAbstractStage::controllerFinishedListener(){
	ControllerWaitCount--;
	if (ControllerWaitCount == 0) { controllerHandled(); }
}

void SPDFAbstractStage::privateOnControllers(SPDFControllerDataList* ParserList, SPDF::SPOLExecutionMode mode) {
	ControllerWaitCount = 0;
	for (auto& i : *ParserList) {
		if (!i.NoWait) { ControllerWaitCount++; }
	}
	onControllers(ParserList, mode);
	if (ControllerWaitCount == 0) { controllerHandled(); }
	delete ParserList;
}