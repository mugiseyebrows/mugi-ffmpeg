#ifndef TASKSMODEL_H
#define TASKSMODEL_H

#include <QStandardItemModel>

class TasksModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum DataRole {
        Id = Qt::UserRole,
        Progress = Qt::UserRole + 1,
        Duration = Qt::UserRole + 2
    };

    explicit TasksModel(QObject *parent = 0);
    
    void append(const QList<QStringList> &args);

    int completeCount() const;

    bool isComplete(int row) const;

    int nextTask();

    void setProgress(int id, double progress);

    QModelIndex findTask(int id);
    QString video(int row);
    QModelIndex findVideo(const QString &path);

    //double estimatedTime(double speed) const;

    double totalProgress() const;

    double duration(bool complete) const;

    double totalDuration() const;
    double completeDuration() const;

    void removeAll();

    void removeComplete();

protected:
    QMap<QString,double> mDuration;
    int mId;

signals:
    void probe(QString);
public slots:
    void onProbed(QString, double);
};

#endif // TASKSMODEL_H
