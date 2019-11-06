#include "fileext.h"

#include <QFileInfo>

QStringList FileExt::videoExts()
{
    return {".avi", ".vfw", ".divx", ".mpg", ".mpeg", ".m1v", ".m2v", ".mpv", ".dv", ".3gp",
        ".mov", ".mp4", ".m4v", ".mqv", ".dat", ".vcd", ".ogg", ".ogm", ".ogv", ".ogx",
        ".asf", ".wmv", ".bin", ".iso", ".vob", ".mkv", ".nsv", ".ram", ".flv", ".rm",
        ".swf", ".ts", ".rmvb", ".dvr-ms", ".m2t", ".m2ts", ".mts", ".rec", ".wtv",
        ".f4v", ".hdmov", ".webm", ".vp8", ".bik", ".smk", ".m4b", ".wtv", ".part"};
}

QStringList FileExt::audioExts()
{
    return {".mp2", ".mp3", ".mpc", ".ogg", ".oga", ".wav", ".wma", ".aac", ".ac3", ".dts",
        ".ra", ".ape", ".flac", ".thd", ".mka", ".m4a", ".wv", ".shn", ".opus"};
}

QStringList FileExt::subtitlesExts()
{
    return {".srt", ".sub", ".ssa", ".ass", ".idx", ".txt", ".smi", ".rt", ".utf", ".aqt", ".vtt"};
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
