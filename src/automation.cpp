#include "automation.h"
#include <QTimer>

Automation *Automation::mInstance = 0;

Automation::Automation(QWidget *widget)
{

}

Automation *Automation::instance(QWidget* widget)
{
    if (!mInstance) {
        mInstance = new Automation(widget);
    }
    return mInstance;
}

void Automation::start()
{
    QTimer::singleShot(0,this,SLOT(onStart()));
}

void Automation::next() {
    start();
}

void Automation::onStart() {

    if (mQueued.isEmpty()) {
        mAction = Action();
        return;
    }

    mAction = mQueued.dequeue();
    if (mAction.type() == Action::ActionEmpty) {

    }

}
