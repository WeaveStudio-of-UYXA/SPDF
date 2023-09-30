﻿#include "../SPSReader.h"

QStringList SPSReader::spawnAllStoryFile(const QString& mainFilePath) {
	//find all js file in the folder that contains the main file, and the subfolder of the folder, but ignore any folder with name "__story__"
	QString FileFolder = mainFilePath.section('/', 0, -2);
	VIConsole::printLine("Root folder: " + FileFolder);
	QStringList allJSFilePath;
	QDirIterator it(FileFolder, QStringList() << "*.js" << "*.sps", QDir::Files, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		QString filePath = it.next();
		if (filePath.contains("__story__")) {
			continue;
		}
		allJSFilePath.append(filePath);
		VIConsole::printLine("Found JS file: " + filePath);
	}
	QStringList allStoryFilePath;
	for (auto i = allJSFilePath.begin(); i != allJSFilePath.end(); i++) {
		VIConsole::printLine("Spawning story file: " + *i);
		allStoryFilePath.append(spawnStoryFile(mainFilePath, *i));
	}
	return allStoryFilePath;
}

QString SPSReader::spawnStoryFile(const QString& mainFilePath, const QString& rawJSFilePath) {
	QString RawFolder = rawJSFilePath.section('/', 0, -2);
	QDir RawDir(RawFolder);
	QString relativePath = RawDir.relativeFilePath(rawJSFilePath);
	QString metaName = relativePath.replace("/", ".").section('.', 0, -2);
	QString FileFolder = rawJSFilePath.section('/', 0, -2);
	QString StoryFileFolder = FileFolder + "/__story__";
	QString StoryFileName = rawJSFilePath.section('/', -1, -1).section('.', 0, -2).replace(".", "_");
	QString StoryFilePath = StoryFileFolder + "/" + StoryFileName;
	if (!QDir(StoryFileFolder).exists()) {
		QDir().mkpath(StoryFileFolder);
	}
	QFile rawFile;
	rawFile.setFileName(rawJSFilePath);
	rawFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream rawFileStream(&rawFile);
	rawFileStream.setCodec("UTF-8");
	bool inSPOLComment = false;
	QStringList storyFileContent;
	while (true) {
		QString line = rawFileStream.readLine();
		if (line.isNull()) {
			break;
		}
		if (line.startsWith("/*SPOL")) {
			inSPOLComment = true;
			continue;
		}
		else if (line.startsWith("SPOL*/")) {
			inSPOLComment = false;
			continue;
		}
		if (inSPOLComment) {
			storyFileContent.append(line);
		}
	}
	rawFile.close();
	if (storyFileContent.length() == 0) {
		return "";
	}
	QString targetJSCode = "//This document is automatically generated by the SPS parser and will be refreshed every time SPDF is started. \
\n//Any changes you make will not be saved.\
\n//此文档由SPS解析器自动生成，每次启动SPDF之前此文件都会被刷新。\n//您的任何更改都不会保存。\n\
export function " + StoryFileName + "_SPOL() {}\n";
	QStringList TagList;
	QString StoryPartName = "__head__";
	QString StoryPartContent = "";
	targetJSCode += "\n" + StoryFileName + "_SPOL." + StoryPartName + " = \n[";
	int lineIndex = 0;
	for (auto i = storyFileContent.begin(); i != storyFileContent.end(); i++) {
		if (i->startsWith("-->")) {
			if (StoryPartName != "") {
				targetJSCode += StoryPartContent + "\"\"];//" + QString::number(lineIndex) + "\n";
				targetJSCode += "SPDF.preloadStory(\""+metaName+"." +StoryPartName+ "\"," + StoryFileName + "_SPOL." + StoryPartName + ");\n";
				lineIndex = 0;
			}
			StoryPartName = i->mid(3, -1);
			StoryPartContent = "";
			targetJSCode += "\n" + StoryFileName + "_SPOL." + StoryPartName + " = \n[";
			continue;
		}
		targetJSCode += "\"" + (*i).replace("\\", "\\\\").replace("\"", "\\\"") + "\", //"+QString::number(lineIndex) + "\n";
		lineIndex++;
	}
	if (StoryPartName != "") {
		targetJSCode += StoryPartContent + "\"\"];//" + QString::number(lineIndex) + "\n";
		targetJSCode += "SPDF.preloadStory(\"" + metaName + "." + StoryPartName + "\"," + StoryFileName + "_SPOL." + StoryPartName + ");\n";
	}

	targetJSCode += "\n";
	QFile targetJSFile;
	targetJSFile.setFileName(StoryFilePath + "_SPOL.js");
	if (targetJSFile.exists()) {
		targetJSFile.remove();
	}
	targetJSFile.open(QIODevice::NewOnly | QIODevice::Text);
	QTextStream targetJSFileStream(&targetJSFile);
	targetJSFileStream.setCodec("UTF-8");
	targetJSFileStream << targetJSCode;
	targetJSFile.close();
	return StoryFilePath;
}