#ifndef DELETEFILTER_H
#define DELETEFILTER_H

#include <QObject>

class QTableView;

class DeleteFilter : public QObject
{
    Q_OBJECT
public:
    enum Action {
        DeleteData,
        RemoveRow,
        RemoveColumn,
        EmitSignal,
        UserAction
    };

    explicit DeleteFilter(QObject *parent = 0);
    
    void setView(QTableView* view);

    void setAction(Action action);

    virtual void deleteAction();

    bool eventFilter(QObject *, QEvent *);

protected:
    QTableView* mView;
    Action mAction;
signals:
    void deletePressed();
public slots:
    
};

#endif // DELETEFILTER_H
