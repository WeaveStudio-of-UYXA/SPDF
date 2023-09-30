#pragma once
#include "SPDFPackageMeta.h"

class SPDFPublicAPI SPOLDocument
{
	_Public QString MetaName;
	_Public QStringList Document;
	_Public def_init SPOLDocument() {};
	_Public def_init SPOLDocument(const QString& name, const QStringList& doc);
};

class SPDFPublicAPI SPOLDocumentManager :public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	_Public QMap<QString, SPOLDocument> Documents;
	VI_PrivateProperty(QString, CurrentDocumentMetaName);
	_Private QStringList* CurrentDocument = nullptr;
	_Private SPOLDocument* CurrentDocumentMeta = nullptr;
	_Private QStringList::iterator CurrentLine;
	_Private bool CurrentLineChanged = false;
	_Public def_init SPOLDocumentManager(VISuper* super);
	_Public void addSPOLDocument(const SPOLDocument& doc);
	_Public void addSPOLDocument(const QString& name, const QStringList& doc);
	_Public void changeCurrentDocument(const QString& name);
	_Public void changeCurrentDocument(const QStringList& doc);
	_Public QStringList getMetaNames();
	_Public bool hasDocument(const QString& name);
	_Public void changeCurrentLineIndex(int index);
	_Public bool currentLineChanged();
	_Public int getCurrentLineIndex();
	_Public int getCurrentDocumentLength();
	_Public QString getCurrentLine();
	_Public QString getLine(int index);
	_Public QString iterateNextLine();
	_Public bool iterateEnd();
};