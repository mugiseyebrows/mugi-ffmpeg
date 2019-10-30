#include "worker.h"

#include <QProcess>
#include <QDir>
#include <QDebug>

#include <QTextCodec>

Worker::Worker(QObject *parent) :
    QObject(parent), mProcess(0), mFinished(true)
{
    mMpeg = "ffmpeg";
    mProbe = "ffprobe";

#ifdef Q_OS_WIN

    QByteArray path_ = qgetenv("PATH");
    QString path = QTextCodec::codecForName("CP866")->toUnicode(path_);
    QStringList paths = path.split(";");

    paths << "C:\\ffmpeg\\bin";

    paths << QDir::homePath() + "\\Documents\\ShareX\\Tools";

    QString p;
    foreach(p,paths) {
        QString p_ = QDir(p).filePath("ffmpeg.exe");
        if (QFile(p_).exists()) {
            mMpeg = QDir::toNativeSeparators(p_);
        }
        p_ = QDir(p).filePath("ffprobe.exe");
        if (QFile(p_).exists()) {
            mProbe = QDir::toNativeSeparators(p_);
        }
    }

#endif
}

bool Worker::isRunning() const
{
    return !mFinished;
}

void Worker::onNextTask(int id, QStringList args, bool overwrite)
{
    mProcess = new QProcess(this);
    mId = id;

    //connect(mProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(onStdoutReady()));
    connect(mProcess,SIGNAL(readyReadStandardError()),this,SLOT(onStderrReady()));
    connect(mProcess,SIGNAL(finished(int)),this,SLOT(onFinished(int)));
    connect(mProcess,SIGNAL(finished(int)),this,SIGNAL(finished()));
    //connect(mProcess,SIGNAL(started()),this,SLOT(onStarted()));
    mProcess->setReadChannel(QProcess::StandardError);

    args.takeFirst();
    mProcess->start(mMpeg,args);

    if (overwrite) {
        mProcess->write("y\n");
    } else {
        mProcess->write("n\n");
    }

    mFinished = false;
}

void Worker::onStderrReady() {

    QStringList lines;
    if (!mProcess)
        return;
    while (mProcess->bytesAvailable()) {
        QByteArray line = mProcess->readLine();
        lines << QString(line).trimmed();
    }
    emit stdErrData(lines.join("\n"));
}

void Worker::onFinished(int)
{
    mFinished = true;
}

void Worker::onTerminate()
{
    if (mProcess) {
        mProcess->kill();
    }
    mFinished = true;
    emit terminated();
}

void Worker::onProbe(QString path)
{
    QProcess p;

    QString cmd = mProbe;
    QStringList args = {"-i",path};

    //p.setReadChannel(QProcess::StandardError);

    p.start(cmd,args);

    p.waitForFinished();

    int ret = p.exitCode();

    if (ret != 0) {
        qDebug() << "error probe return code" << ret;
    }

    QString s = p.readAllStandardError();

    QStringList lines = s.split("\n");
    QString line;

    QRegExp rxstream("Stream #([0-9]+):([0-9]+)");
    QRegExp rxduration("DURATION\\s*:\\s*([0-9]+):([0-9]+):([0-9]+)([.0-9]+)", Qt::CaseInsensitive);

    QMap<int,double> streams;

    int index = -1; // duration before streams

    foreach(line,lines) {
        line = line.trimmed();
        if (line.indexOf(rxstream) > -1) {
            index = rxstream.cap(2).toInt();
        }
        if (line.indexOf(rxduration) > -1) {

            double duration =
                    rxduration.cap(1).toInt() * 3600 +
                    rxduration.cap(2).toInt() * 60 +
                    rxduration.cap(3).toInt();

            if (streams.contains(index)) {
                qDebug() << "probe parse error";
            } else {
                streams[index] = duration;
                //qDebug() << "index" << index << "duration" << duration;
            }
        }
    }

    if (streams.contains(-1)) {
        emit probed(path,streams[-1]);
    } else if (streams.contains(0)) {
        emit probed(path,streams[0]);
    } else {
        qDebug() << "probe error stream 0:0 not found";
    }

}

