#include "../SPOLDocument.h"

def_init SPOLDocument::SPOLDocument(const QString& name, const QStringList& doc) {
	MetaName = name;
	Document = doc;
}

void SPOLDocumentManager::addSPOLDocument(const SPOLDocument& doc) {
	if (!Documents.contains(doc.MetaName)) {
		Documents.insert(doc.MetaName, doc);
	}
}

void SPOLDocumentManager::addSPOLDocument(const QString& name, const QStringList& doc) {
	SPOLDocument docobj(name, doc);
	addSPOLDocument(docobj);
}

void SPOLDocumentManager::changeCurrentDocument(const QString& name) {
	if (Documents.contains(name)) {
		CurrentDocument = &Documents[name].Document;
		CurrentDocumentMeta = &Documents[name];
		CurrentLine = CurrentDocument->begin();
		CurrentLineChanged = false;
		CurrentDocumentMetaName = name;
	}
}

QStringList SPOLDocumentManager::getMetaNames() {
	return Documents.keys();
}

bool SPOLDocumentManager::hasDocument(const QString& name) {
	return Documents.contains(name);
}

void SPOLDocumentManager::changeCurrentLineIndex(int index) {
	if (CurrentDocument != nullptr) {
		if (index >= 0 && index < CurrentDocument->size()) {
			CurrentLine = CurrentDocument->begin() + index;
			CurrentLineChanged = true;
		}
	}
}

int SPOLDocumentManager::getCurrentLineIndex() {
	if (CurrentDocument != nullptr) {
		return CurrentLine - CurrentDocument->begin();
	}
}

QString SPOLDocumentManager::getCurrentLine() {
	if (CurrentDocument != nullptr) {
		return *CurrentLine;
	}
}

QString SPOLDocumentManager::getLine(int index) {
	if (CurrentDocument != nullptr) {
		if (index >= 0 && index < CurrentDocument->size()) {
			return CurrentDocument->at(index);
		}
	}
}

QString SPOLDocumentManager::iterateNextLine() {
	if (CurrentDocument != nullptr) {
		CurrentLineChanged = false;
		QString rtn = *CurrentLine;
		CurrentLine++;
		return rtn;
	}
}