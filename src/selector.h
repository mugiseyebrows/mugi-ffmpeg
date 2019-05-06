#ifndef SELECTOR_H
#define SELECTOR_H

#include <QStringList>
#include <QObject>

class Selector : public QObject
{
    Q_OBJECT
public:
    Selector(QObject* parent = 0);

    bool setInputs(const QString& input1, const QString& input2);
    void setOutput(const QString& output);

    bool twoInputs() const;

    bool isEmpty() const;

    QList<QList<QStringList> > tasks(const QStringList& options, const QList<bool>& checkList) const;

    QStringList names() const;

    //void tasks(const QStringList &options, const QList<bool> &checkList, QList<QList<QStringList> > &result, QStringList &quoted) const;
    QString quoted(const QStringList &task) const;
    void tasks(const QStringList &options, const QList<bool> &checkList, QList<QStringList> &args, QStringList &quoted) const;
protected:

    QStringList mNames;
    QList<QStringList> mInputs;
    QStringList mOutputs;

};

#endif // SELECTOR_H
