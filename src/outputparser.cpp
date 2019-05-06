#include "outputparser.h"

#include <QRegExp>
#include <QStringList>

OutputParser::OutputParser(QObject *parent) :
    QObject(parent)
{
    clear();
}

void OutputParser::clear()
{
    mDuration = -1;
    mTime = -1;
    mSpeed = -1;
}

int OutputParser::parseToSeconds(const QStringList& caps) {

    return caps[1].toInt() * 3600
            + caps[2].toInt() * 60
            + caps[3].toInt();
}

void OutputParser::parse(const QString &line)
{
    QRegExp duration("Duration: (\\d+):(\\d+):(\\d+)\\.(\\d+)");
    QRegExp time("time=(\\d+):(\\d+):(\\d+)\\.(\\d+)");
    QRegExp speed("speed=([0-9.]+)x");

    if (line.indexOf(duration) > -1) {
        mDuration = parseToSeconds(duration.capturedTexts());
    } else if (line.lastIndexOf(time) > -1) {
        mTime = parseToSeconds(time.capturedTexts());
    }
    if (line.indexOf(speed) > -1) {
        mSpeed = speed.cap(1).toDouble();
    }
}

int OutputParser::duration() const
{
    return mDuration;
}

int OutputParser::time() const
{
    return mTime;
}

double OutputParser::speed() const
{
    return mSpeed;
}
