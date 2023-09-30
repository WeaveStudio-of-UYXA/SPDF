#pragma once
#include "SPDFPackageMeta.h"
#include "SPOLAbstractParser.h"
class SPDFPublicAPI SPOLAbstractSegment :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	friend class SPOLInterpreter;
	_Public SPOLInterpreter* Interpreter;
	_Protected QStack<qint32> FirstIndentStack;
	_Protected QStack<qint32> SegmentIndentStack;
	_Public QString SegmentName;
	_Public virtual void onParseLine(const QString& line, SPDF::SPOLExecutionMode mode) HalfVirtual;
	_Public virtual bool onIndentMinus() { return true; }
};

typedef QHash<QString, QVariant> ParamHash;
class SPDFPublicAPI SPOLSegment_FUNC :public SPOLAbstractSegment
{
	Q_OBJECT;
	VI_OBJECT;
	friend class SPOLInterpreter;
	_Protected QStack<ParamHash> ParamStack;
	_Protected QStack<QString> FuncNameStack;
	_Protected QStack<qint32> JumpStack;
	_Protected QMap<QString, qint32> FuncNameMap;
	_Public virtual void onParseLine(const QString& line, SPDF::SPOLExecutionMode mode) override {}
	_Public virtual bool onIndentMinus() override {}
};

class SPDFPublicAPI SPOLSegment_Global :public SPOLAbstractSegment
{
	Q_OBJECT;
	VI_OBJECT;
	_Public def_init SPOLSegment_Global() {
		FirstIndentStack.push(-4);
		SegmentIndentStack.push(0);
	}
};

class SPDFPublicAPI SPOLSegment_SWITCH :public SPOLAbstractSegment
{
	Q_OBJECT;
	VI_OBJECT;
	_Public def_init SPOLSegment_SWITCH() {
		
	}
	_Public virtual void onParseLine(const QString& line, SPDF::SPOLExecutionMode mode) override;
	_Public virtual bool onIndentMinus() override;
};