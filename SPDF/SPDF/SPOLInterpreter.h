#pragma once
#include "SPDFParser.h"
#include "SPOLAbstractSegment.h"
typedef QMap<QString, QString> SPOLContext;
typedef QMap<QString, SPDFAbstractControllerParser*> SPDFParserMap;

/*
SPOL解释器
SPOL解释器在VIECMAScript的线程中运行，请勿进行危险操作
*/

class VIECMA_SPOL;
class SPDFPublicAPI SPOLInterpreter :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	friend class VIECMA_SPOL;
	friend class SPOLAbstractSegment;
	_Signal void sceneFinished(SPDF::SPOLExecutionMode mode);
	_Signal void spolDocumentChanged(const QStringList& spol, SPDF::SPOLExecutionMode mode);
	_Signal void onControllers(SPDFParserResultList* list, SPDF::SPOLExecutionMode mode);
	_Private SPDFAbstractTerminal* Terminal;
	_Private SPDFParserMap Parsers;
	_Private QList<QString> SPOLDocument;
	_Private SPOLContext SPOLDocumentContext;
	_Private bool SceneFinished;
	_Private QMutex* ThreadMutex;
	_Private QWaitCondition* ThreadWaitCondition;
	_Private bool lineChanged = false;
	_Private QList<QString>::Iterator CurrentLine;
	_Protected QMap<QString, QStringList> SPOLDocumentMap;
	VI_ProtectedProperty(QString, CurrentMetaName);
	_Private QHash<QString, QVariant> Variables;
	_Private QStack<SPOLAbstractSegment*> SegmentStack;
	_Private QList<SPOLAbstractSegment*> SegmentParserList;
	_Private bool allowIndentAdd = false;
	_Private qint32 CurrentIndent = 0;
	_Public def_init SPOLInterpreter(SPDFAbstractTerminal* terminal, QMutex* mutex, QWaitCondition* waitCondition);
	_Public def_del ~SPOLInterpreter();
	_Public void addParser(SPDFAbstractControllerParser* parser);
	_Public void addSPOL(const QString& metaName, const QStringList& spol);
	_Public QString getSPOLWithIndex(unsigned int index);
	_Public quint32 getCurrentSPOLDocumentLength();
	_Public unsigned long long getExecuteLineIndex();
	_Public void changeExecuteLine(unsigned int index);
	_Public void setVariable(const QString& name, const QVariant& value);
	_Public QVariant getVariable(const QString& name);
	_Public void executeSPOL(SPDF::SPOLExecutionMode mode, const QStringList& spol);
	_Public void executeSPOL(SPDF::SPOLExecutionMode mode, const QString& metaName);
	_Public void executeSPOLSingleLine(const QString& line, SPDF::SPOLExecutionMode mode = SPDF::SPOLExecutionMode::StepDebug);
	_Private qint32 checkIndent(QStringList::iterator& line);
	_Public static qint32 indentCount(const QString& line);
	_Public static qint32 indentCharCount(const QString& line);
	_Public void popSegment();
	_Private void wait();
};