#include "sourcesmodel.h"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "fileext.h"
#include "namematcher.h"

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
    qSort(result.begin(), result.end());
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

bool SourcesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    for(int i=0;i<count;i++) {
        mFiles.removeAt(row);
    }
    bool ret = QStandardItemModel::removeRows(row,count,parent);

    qDebug() << "removeRows";
    updateMapping();
    emit sourceRemoved();
    return ret;
}

bool SourcesModel::insertRows(int row, int count, const QModelIndex &parent)
{
    for (int i=0;i<count;i++) {
        mFiles.insert(row,QStringList());
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

void SourcesModel::appendRow(Type type, const QString& path, const QStringList& files) {
    int row = rowCount();
    insertRow(row);
    setData(this->index(rowCount()-1,ColumnType),type);
    setData(this->index(rowCount()-1,ColumnPath),path);
    mFiles[row] = files;
    updateMapping();
}

void SourcesModel::appendRow(Type type, const QString& path) {

    QStringList files = findFiles(path,type);
    appendRow(type, path, files);
}

QMap<int,int> toMap(const QList<QPair<int,int> >& vs) {
    QMap<int,int> result;
    for(int i=0;i<vs.size();i++) {
        result[vs[i].first] = vs[i].second;
    }
    return result;
}

void SourcesModel::updateMapping() {
    mMapping.clear();
    for(int i=1;i<mFiles.size();i++) {
        mMapping.append(toMap(NameMatcher::match(mFiles[0], mFiles[i])));
    }
    updateIndexes();
}

void SourcesModel::updateIndexes() {

    mIndexes.clear();

    if (mFiles.size() == 0) {
        return;
    } else if (mFiles.size() == 1) {
        for(int i=0;i<mFiles[0].size();i++) {
            mIndexes << i;
        }
    } else {
        for(int i=0;i<mFiles[0].size();i++) {
            bool ok = true;
            for(int j=0;j<mMapping.size();j++) {
                if (!mMapping[j].contains(i)) {
                    ok = false;
                }
            }
            if (ok) {
                mIndexes << i;
            }
        }
    }

}

QStringList SourcesModel::files() const
{
    QStringList result;
    foreach (int index, mIndexes) {
        result << QFileInfo(mFiles[0][index]).baseName();
    }
    return result;
}

QString replace(const QString& subj, const QString& needle, const QString& repl) {
    QString subj_ = subj;
    subj_.replace(needle, repl);
    return subj_;
}

QList<QStringList> SourcesModel::tasks(const QList<int> &checked,
                                       const QStringList &options,
                                       const QString &output) const
{
    QList<QStringList> res;
    foreach (int i, checked) {
        int index = mIndexes[i];
        QStringList cmd;
        cmd << "ffmpeg";
        cmd << "-i" << QDir::toNativeSeparators(mFiles[0][index]);
        for(int j=0;j<mMapping.size();j++) {
            int index2 = mMapping[j][index];
            cmd << "-i" << QDir::toNativeSeparators(mFiles[j+1][index2]);
        }
        cmd << options;
        cmd << replace(output, "%name%", QFileInfo(mFiles[0][index]).baseName());
        res << cmd;
    }
    return res;
}

