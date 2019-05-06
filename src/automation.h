#ifndef AUTOMATION_H
#define AUTOMATION_H

#include <QObject>
#include <QQueue>
#include "action.h"

class Automation : public QObject
{
    Q_OBJECT
public:
    static Automation* instance(QWidget *widget);

    void start();
    void next();
protected:
    Automation(QWidget *widget);
    static Automation* mInstance;

    QWidget* mWidget;

    QQueue<Action> mQueued;
    Action mAction;

protected slots:

    void onStart();
};

#endif // AUTOMATION_H
