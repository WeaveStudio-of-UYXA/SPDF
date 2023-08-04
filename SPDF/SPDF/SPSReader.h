#pragma once
#include "../Visindigo/VICore"
/*

*/
class SPSReader : public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	VI_STATIC_CLASS (SPSReader);
	_Public static QStringList spawnAllStoryFile(const QString& mainFilePath);
	_Public static QString spawnStoryFile(const QString& rawJSFilePath);
};