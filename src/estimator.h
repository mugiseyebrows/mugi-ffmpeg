#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include <QTime>

class Measurement {
public:
    Measurement(double total, double complete, const QTime& time) :
        total(total), complete(complete), time(time) {}
    double total;
    double complete;
    QTime time;
};

class Estimator
{
public:
    Estimator();

    bool needData() const;

    void setData(double total, double complete);

    QString time() const;

    QString estimated() const;

    void clear();

    static double secsTo(const QTime& t1, const QTime& t2);
    static QString secsToString(double secs);

protected:

    static const int mInterval;

    QList<Measurement> mData;
    QTime mStarted;

};

#endif // ESTIMATOR_H
