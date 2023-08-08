#pragma once
#include "SPDFParser.h"

typedef QMap<QString, QString> SPOLContext;
typedef QMap<QString, SPDFAbstractControllerParser*> SPDFParserMap;

/*
SPOL解释器
SPOL解释器在VIECMAScript的线程中运行，请勿进行危险操作
*/

class SPDFPublicAPI SPOLInterpreter :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	_Signal void sceneFinished();
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
	_Private QHash<QString, QVariant> Variables;
	_Public def_init SPOLInterpreter(SPDFAbstractTerminal* terminal, QMutex* mutex, QWaitCondition* waitCondition);
	_Public def_del ~SPOLInterpreter();
	_Public void addParser(SPDFAbstractControllerParser* parser);
	_Private void wait();
	_Public void executeSPOL(SPDF::SPOLExecutionMode mode, const QStringList& spol);
	_Public unsigned long long getExecuteLineIndex();
	_Public void changeExecuteLine(unsigned int index);
	_Public void setVariable(const QString& name, const QVariant& value);
	_Public QVariant getVariable(const QString& name);
	_Public QString getSPOLWithIndex(unsigned int index);
	_Public void executeSPOL(SPDF::SPOLExecutionMode mode, const QString& spol = "");
};