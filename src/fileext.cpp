#include "fileext.h"

#include "lit.h"
#include <QFileInfo>

QStringList FileExt::videoExts()
{
    return Lit::sl(".mkv",".mp4",".ts",".avi",".wmv",".mpg");
}

QStringList FileExt::audioExts()
{
    return Lit::sl(".mka",".mp3",".flac",".aac",".ac3");
}

QStringList FileExt::subtitlesExts()
{
    return Lit::sl(".srt",".ass",".ssa");
}

bool FileExt::isVideo(const QString &path)
{
    return videoExts().contains("." + QFileInfo(path).suffix());
}

bool FileExt::isAudio(const QString &path)
{
    return audioExts().contains("." + QFileInfo(path).suffix());
}

bool FileExt::isSubtitles(const QString &path)
{
    return subtitlesExts().contains("." + QFileInfo(path).suffix());
}
