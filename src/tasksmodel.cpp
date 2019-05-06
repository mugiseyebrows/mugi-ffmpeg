#include "tasksmodel.h"

#include <QFileInfo>
#include <QDebug>

#include "fileext.h"

TasksModel::TasksModel(QObject *parent) :
    QStandardItemModel(0,1,parent), mId(0)
{

}

void TasksModel::append(const QList<QStringList>& args)
{
    if (args.size() < 1)
        return;
    int rowCount = this->rowCount();
    insertRows(rowCount,args.size());
    for(int i=0;i<args.size();i++) {
        QModelIndex index = this->index(rowCount+i,0);
        setData(index,mId++,Id);
        setData(index,args[i]);
        setData(index,0.0,Progress);
    }
    for(int i=0;i<this->rowCount();i++) {
        if (data(index(i,0),Duration).isNull()) {
            QString video = this->video(i);
            if (video.isEmpty()) {
                qDebug() << "error: video.isEmpty()";
            } else {
                emit probe(video);
            }
        }
    }
}

int TasksModel::completeCount() const
{
    int complete = 0;
    for (int i=0;i<rowCount();i++) {
        if (isComplete(i))
            complete++;
    }
    return complete;
}

bool TasksModel::isComplete(int row) const
{
    return data(index(row,0),Progress).toDouble() == 1.0;
}

int TasksModel::nextTask() {
    for (int i=0;i<rowCount();i++) {
        if (!isComplete(i))
            return data(this->index(i,0),Id).toInt();
    }
    return -1;
}

QModelIndex TasksModel::findTask(int id) {
    for (int i=0;i<rowCount();i++) {
        if (data(this->index(i,0),Id).toInt() == id)
            return this->index(i,0);
    }
    return QModelIndex();
}

void TasksModel::setProgress(int id, double progress)
{
    QModelIndex index = findTask(id);
    if (!index.isValid()) {
        qDebug() << "setProgress error: invalid index";
        return;
    }
    setData(index,progress,Progress);
}

QString TasksModel::video(int row) {
    QStringList sl = data(index(row,0)).toStringList();
    for(int i=0;i<sl.size()-1;i++) {
        if (sl[i] == "-i") {
            if (FileExt::isVideo(sl[i+1]))
                return sl[i+1];
        }
    }
    return QString();
}

QModelIndex TasksModel::findVideo(const QString& path) {
    for (int i=0;i<rowCount();i++) {
        if (video(i) == path)
            return index(i,0);
    }
    return QModelIndex();
}

double TasksModel::totalProgress() const
{
    if (rowCount() < 1)
        return 1.0;

    double ps = 0.0;
    for(int i=0;i<rowCount();i++) {
        QModelIndex idx = index(i,0);
        double p = data(idx,Progress).toDouble();
        ps += p;
    }
    return ps / rowCount();
}

double TasksModel::duration(bool complete) const
{
    double s = 0.0;
    for(int i=0;i<rowCount();i++) {
        QModelIndex idx = index(i,0);
        double d = data(idx,Duration).toDouble();
        if (d > 0) {
            d *= complete ? data(idx,Progress).toDouble() : 1.0;
            qDebug() << "complete" << complete << "duration" << d;
            s += d;
        } else {
            qDebug() << "TasksModel duration error";
        }
    }
    return s;
}

double TasksModel::totalDuration() const
{
    return duration(false);
}

double TasksModel::completeDuration() const
{
    return duration(true);
}

void TasksModel::removeAll()
{
    removeRows(0,rowCount());
}

void TasksModel::removeComplete()
{
    for(int i=rowCount()-1;i>=0;i--) {
        if (isComplete(i))
            removeRow(i);
    }
}

void TasksModel::onProbed(QString path, double duration)
{
    for(int i=0;i<rowCount();i++) {
        if (video(i) == path) {
            setData(index(i,0),duration,Duration);
        }
    }
}
