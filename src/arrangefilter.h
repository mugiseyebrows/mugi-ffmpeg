#ifndef ARRANGEFILTER_H
#define ARRANGEFILTER_H

#include <QObject>

class QTableView;

class QAbstractItemModel;

class ArrangeFilter : public QObject
{
    Q_OBJECT
public:
    explicit ArrangeFilter(QObject *parent = 0);
    
    void setView(QTableView* view);
    void setEnabled(bool value);
    void setRoles(const QList<int> &roles);

    bool eventFilter(QObject *, QEvent *);

    virtual void swapRows(QAbstractItemModel *m, int row1, int row2);

protected:
    QTableView* mView;
    bool mEnabled;

    bool mDrag;
    int mDragRow;

    QList<int> mRoles;

signals:
    
public slots:
    
};

#endif // ARRANGEFILTER_H
