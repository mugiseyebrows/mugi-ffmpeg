#include "estimator.h"

#define INTERVAL 30

Estimator::Estimator()
{
}

bool Estimator::needData() const
{
    if (mData.size() == 0)
        return true;
    if (secsTo(mData.last().time,QTime::currentTime()) > INTERVAL) {
        return true;
    }
    return false;
}

void Estimator::setData(double total, double complete)
{
    mData << Measurement(total,complete,QTime::currentTime());
    if (mData.size() == 1)
        mStarted = mData[0].time;
    if (mData.size() > 2)
        mData.removeAt(0);
}

QString Estimator::time() const
{
    if (mData.isEmpty())
        return QString();
    return secsToString(secsTo(mStarted, QTime::currentTime()));
}

QString Estimator::estimated() const
{
    if (mData.size() < 2)
        return QString();
    double speed = (mData[1].complete - mData[0].complete) / secsTo(mData[0].time, mData[1].time);
    double path = (mData[1].total - mData[1].complete);
    double secs = path / speed - secsTo(mData[1].time, QTime::currentTime());
    return secsToString(secs);
}

void Estimator::clear()
{
    mData.clear();
}

QString Estimator::secsToString(double secs) {
    return QTime(0,0,0).addSecs(secs).toString("hh:mm:ss");
}

double Estimator::secsTo(const QTime &t1, const QTime &t2)
{
    return t1.secsTo(t2);
}
