#ifndef TESKITEMDELEGATE_H
#define TESKITEMDELEGATE_H

#include <QStyledItemDelegate>

class TaskItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TaskItemDelegate(QObject *parent = 0);
    
    QString displayText(const QVariant &value, const QLocale &locale) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:

signals:
    
public slots:
    
};

#endif // TESKITEMDELEGATE_H
