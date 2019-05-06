#include "sourcesmodel.h"


#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "fileext.h"

SourcesModel::SourcesModel(QObject *parent) :
    QStandardItemModel(0,2,parent)
{

}

QStringList SourcesModel::findFiles(const QString& path, const QStringList& exts) {
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

QStringList SourcesModel::findFiles(const QString& path, Type type) {
    if (type == Video)
        return findFiles(path, FileExt::videoExts());
    else if (type == Audio)
        return findFiles(path, FileExt::audioExts());
    else if (type == Subtitles)
        return findFiles(path, FileExt::subtitlesExts());
    return QStringList();
}

QMap<QString,QString> SourcesModel::mapBaseNamePath(const QStringList& files) {
    QMap<QString,QString> res;
    QString path;
    foreach(path,files) {
        res[QFileInfo(path).completeBaseName()] = path;
    }
    return res;
}


bool SourcesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    for(int i=0;i<count;i++) {
        mFiles.removeAt(row);
    }
    bool ret = QStandardItemModel::removeRows(row,count,parent);
    emit sourceRemoved();
    qDebug() << "removeRows";
    return ret;
}

bool SourcesModel::insertRows(int row, int count, const QModelIndex &parent)
{
    for (int i=0;i<count;i++) {
        mFiles.insert(row,QMap<QString,QString>());
    }
    return QStandardItemModel::insertRows(row,count,parent);
}

void SourcesModel::removeAll()
{
    removeRows(0,rowCount());
}

SourcesModel::Type SourcesModel::type(int row)
{
    return (Type) data(index(row,ColumnType)).toInt();
}

void SourcesModel::addVideo(const QString &path)
{
    appendRow(Video,path);
}

void SourcesModel::addAudio(const QString &path)
{
    appendRow(Audio,path);
}

void SourcesModel::addSubtitles(const QString &path)
{
    appendRow(Subtitles,path);
}

void SourcesModel::addSource(SourcesModel::Type type, const QString &path)
{
    appendRow(type, path);
}

void SourcesModel::appendRow(Type type, const QString& path) {
    int row = rowCount();
    insertRow(row);
    setData(this->index(rowCount()-1,ColumnType),type);
    setData(this->index(rowCount()-1,ColumnPath),path);
    mFiles[row] = mapBaseNamePath(findFiles(path,type));
}

QStringList SourcesModel::files() const
{
    if (mFiles.size() == 0)
        return QStringList();

    QStringList keys = mFiles[0].keys();
    if (mFiles.size() == 1) {
        qSort(keys);
        return keys;
    } else {
        qSort(keys);
        QStringList res;
        QString key;

        foreach(key,keys) {
            bool skip = false;
            for(int i=1;i<mFiles.size();i++) {
                if (!mFiles[i].contains(key)) {
                    skip = true;
                    break;
                }
            }
            if (!skip)
                res << key;
        }

        return res;
    }

    return QStringList();
}

QList<QStringList> SourcesModel::tasks(const QStringList &checked,
                                       const QStringList &options,
                                       const QString &output) const
{
    QList<QStringList> res;
    QString item;
    foreach(item,checked) {
        QStringList cmd;
        QString output_ = output;
        cmd << "ffmpeg";
        for(int i=0;i<mFiles.size();i++) {
            cmd << "-i" << QDir::toNativeSeparators(mFiles[i][item]);
        }
        cmd << options;
        cmd << output_.replace("%name%",item);
        res << cmd;
    }
    return res;
}

