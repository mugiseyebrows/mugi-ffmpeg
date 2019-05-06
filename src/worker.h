#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QStringList>

class QProcess;

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);

    //static QString quoted(const QString &cmd, const QStringList &args);

    bool isRunning() const;

signals:

    void finished();
    void stdErrData(QString);
    void probed(QString,double);
    void terminated();

protected:
    QProcess* mProcess;

    QString mMpeg;
    QString mProbe;

    bool mFinished;

    int mId;

public slots:
    
    void onProbe(QString);
    void onNextTask(int id,QStringList args, bool overwrite);
    void onStderrReady();
    void onFinished(int);
    void onTerminate();
};

#endif // WORKER_H
