#ifndef FILEEXT_H
#define FILEEXT_H

#include <QStringList>

class FileExt
{
public:

    static QStringList videoExts();
    static QStringList audioExts();
    static QStringList subtitlesExts();

    static bool isVideo(const QString& path);
    static bool isAudio(const QString& path);
    static bool isSubtitles(const QString& path);

};

#endif // FILEEXT_H
