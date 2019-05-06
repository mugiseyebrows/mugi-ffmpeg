#include "arrangefilter.h"

#include <QTableView>
#include <QMouseEvent>
#include <QDebug>

ArrangeFilter::ArrangeFilter(QObject *parent) :
    QObject(parent), mView(0), mEnabled(false)
{
    mRoles << Qt::DisplayRole;
}

void ArrangeFilter::setView(QTableView *view)
{
    mView = view;
    //mView->installEventFilter(this);
    mView->viewport()->installEventFilter(this);
}

void ArrangeFilter::setEnabled(bool value)
{
    mEnabled = value;
}

void ArrangeFilter::setRoles(const QList<int>& roles)
{
    mRoles = roles;
}

void ArrangeFilter::swapRows(QAbstractItemModel* m, int row1, int row2) {
    for (int i=0;i<m->columnCount();i++) {
        QVariantList vs;
        int role;
        foreach(role,mRoles) {
            vs << m->data(m->index(row1,i),role);
        }
        foreach(role,mRoles) {
            m->setData(m->index(row1,i),m->data(m->index(row2,i),role),role);
        }
        for(int j=0;j<mRoles.size();j++) {
            m->setData(m->index(row2,i),vs[j], mRoles[j]);
        }
    }
}

bool ArrangeFilter::eventFilter(QObject* obj, QEvent * e)
{
    if (!mEnabled || !mView)
        return QObject::eventFilter(obj,e);

    if (e->type() == QEvent::MouseButtonPress && obj == mView->viewport()) {

        QMouseEvent* me = static_cast<QMouseEvent*>(e);
        QModelIndex idx = mView->indexAt(me->pos());
        if (idx.isValid()) {
            mDrag = true;
            mDragRow = idx.row();
        }

        //qDebug() << "MouseButtonPress";
        return false;
    } else if (e->type() == QEvent::MouseMove && obj == mView->viewport()) {

        if (mDrag) {
            QMouseEvent* me = static_cast<QMouseEvent*>(e);
            //qDebug() << "MouseMove" << me->pos() << mView->indexAt(me->pos());
            QModelIndex idx = mView->indexAt(me->pos());
            if (idx.isValid() && idx.row() != mDragRow) {

                QAbstractItemModel* m = mView->model();

                swapRows(m,mDragRow,idx.row());
                mDragRow = idx.row();

                QItemSelection selection(m->index(idx.row(),0),m->index(idx.row(),m->columnCount()-1));

                mView->selectionModel()->select(selection,QItemSelectionModel::ClearAndSelect);
            }
            return true;
        }


    } else if (e->type() == QEvent::MouseButtonRelease && obj == mView->viewport()) {
        //qDebug() << "MouseButtonRelease";

        if (mDrag) {
            mDrag = false;
            return true;
        }

    }

    return QObject::eventFilter(obj,e);
}

