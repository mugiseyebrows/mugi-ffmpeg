#include "deletefilter.h"

#include <QTableView>

#include <QKeyEvent>

DeleteFilter::DeleteFilter(QObject *parent) :
    QObject(parent), mView(0), mAction(DeleteData)
{
}

void DeleteFilter::setView(QTableView *view)
{
    view->installEventFilter(this);
    mView = view;
}

void DeleteFilter::setAction(DeleteFilter::Action action)
{
    mAction = action;
}

void DeleteFilter::deleteAction()
{
    QAbstractItemModel* m = mView->model();
    if (mAction == DeleteData) {
        QModelIndexList idxs = mView->selectionModel()->selectedIndexes();
        QModelIndex idx;
        foreach(idx,idxs) {
            m->setData(idx,QVariant());
        }
    } else if (mAction == RemoveRow) {
        QModelIndex idx = mView->currentIndex();
        if (!idx.isValid())
            return;
        m->removeRow(idx.row());
    } else if (mAction == RemoveColumn) {
        QModelIndex idx = mView->currentIndex();
        if (!idx.isValid())
            return;
        m->removeRow(idx.row());
    } else if (mAction == EmitSignal) {
        emit deletePressed();
    } else if (mAction == UserAction) {

    }
}

bool DeleteFilter::eventFilter(QObject * o, QEvent * e)
{
    if (o == mView && e->type() == QEvent::KeyPress) {
        if (static_cast<QKeyEvent*>(e)->key() == Qt::Key_Delete) {
            deleteAction();
            return true;
        }
    }
    return QObject::eventFilter(o,e);
}

