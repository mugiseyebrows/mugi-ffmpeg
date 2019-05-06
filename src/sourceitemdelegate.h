#ifndef SOURCEITEMDELEGATE_H
#define SOURCEITEMDELEGATE_H

#include <QStyledItemDelegate>

class SourceItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SourceItemDelegate(QObject *parent = 0);
    
    QString displayText(const QVariant &value, const QLocale &locale) const;

signals:
    
public slots:
    
};

#endif // SOURCEITEMDELEGATE_H
