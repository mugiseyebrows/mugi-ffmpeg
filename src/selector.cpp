#include "selector.h"

#include <QFile>
#include <QDir>

Selector::Selector(QObject* parent) : QObject(parent)
{
}

QStringList stringList(const QString& item1) {
    QStringList result;
    result << item1;
    return result;
}

QStringList stringList(const QString& item1, const QString& item2) {
    QStringList result;
    result << item1 << item2;
    return result;
}

QStringList findFiles(const QString& path, const QStringList& exts) {
    QDir dir(path);
    QStringList entryList = dir.entryList();
    QStringList result;
    QString file;
    QString ext;
    foreach(file,entryList) {
        QString file_ = file.toLower();
        foreach(ext,exts) {
            if (file_.endsWith(ext)) {
                result << dir.filePath(file);
                break;
            }
        }
    }
    return result;
}


bool Selector::setInputs(const QString &input1, const QString &input2)
{
    mNames.clear();
    mInputs.clear();
    mOutputs.clear();

    if (input1.isEmpty())
        return false;

    QStringList videoExts;
    QStringList audioExts;
    videoExts << ".mkv" << ".mp4" << ".ts" << ".avi";
    audioExts << ".mka";

    if (QDir(input1).exists()) {
        // directory mode

        QStringList videoFiles = findFiles(input1,videoExts);
        if (!input2.isEmpty() && QDir(input2).exists()) {
            // two inputs for each task
            QStringList audioFiles = findFiles(input2,audioExts);

            QString path;
            QStringList videoFiles_;
            QStringList audioFiles_;
            foreach(path,videoFiles) {
                videoFiles_ << QFileInfo(path).completeBaseName();
            }
            foreach(path,audioFiles) {
                audioFiles_ << QFileInfo(path).completeBaseName();
            }

            for (int i=0;i<videoFiles_.size();i++) {
                for (int j=0;j<audioFiles_.size();j++) {
                    if (videoFiles_[i] == audioFiles_[j]) {
                        mInputs << stringList(videoFiles[i],audioFiles[j]);
                    }
                }
            }
            QStringList paths;
            foreach(paths,mInputs) {
                mNames << QFileInfo(paths[0]).completeBaseName();
            }

        } else {
            // one input for each task
            QString path;
            foreach(path,videoFiles) {
                mInputs << stringList(path);
            }
            QStringList paths;
            foreach(paths,mInputs) {
                mNames << QFileInfo(paths[0]).fileName();
            }
        }

    } else if (QFile::exists(input1)) {
        // file mode
        if (!input2.isEmpty() && QFile::exists(input2)) {
            // two inputs for each task
            mInputs << stringList(input1,input2);
            mNames << QFileInfo(input1).completeBaseName();
        } else {
            // one input for each task
            mInputs << stringList(input1);
            mNames << QFileInfo(input1).fileName();
        }
    }
    return mInputs.size() > 0;
}

void Selector::setOutput(const QString &output)
{
    mOutputs.clear();
    QStringList paths;
    QFileInfo fileInfo(output);
    QDir dir = fileInfo.dir();
    QString fileMask = fileInfo.fileName();
    foreach(paths,mInputs) {
        QString fileName = fileMask;
        fileName.replace("%name%", QFileInfo(paths[0]).completeBaseName());
        QString path = dir.filePath(fileName);
        mOutputs << path;
    }
}

bool Selector::isEmpty() const {
    return mInputs.isEmpty();
}

QString Selector::quoted(const QStringList& task) const {
    QStringList res;
    QString e;
    res << "ffmpeg";
    foreach(e,task) {
        if (e.indexOf(" ") > -1)
            res << "\"" + e + "\"";
        else
            res << e;
    }
    return res.join(" ");
}

void Selector::tasks(const QStringList& options, const QList<bool>& checkList, QList<QStringList>& args, QStringList& quoted) const
{
    for (int i=0;i<mInputs.size();i++) {
        if (!checkList[i])
            continue;
        QStringList args_;
        QString e;
        foreach(e,mInputs[i])
            args_ << "-i" << e;
        args_.append(options);
        args_ << mOutputs[i];

        args << args_;
        quoted.append( this->quoted(args_) );
    }
}

QStringList Selector::names() const
{
    return mNames;
}

bool Selector::twoInputs() const
{
    if (isEmpty())
        return false;
    return mInputs[0].size() > 1;
}
