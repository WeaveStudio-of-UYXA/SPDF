#pragma once
#include <VICore>
#include "private/SPDFCompileMacro.h"

class SPDFPublicAPI SPSReader : public VIObject
{
	Q_OBJECT;
	VI_OBJECT;
	VI_STATIC_CLASS (SPSReader);
	_Public static QStringList spawnAllStoryFile(const QString& mainFilePath);
	_Public static QString spawnStoryFile(const QString& mainFilePath, const QString& rawJSFilePath);
};