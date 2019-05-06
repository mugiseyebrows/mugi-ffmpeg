#ifndef PROGRESSTABLE_H
#define PROGRESSTABLE_H

#include <QTableView>
class QProgressBar;
class ProgressBar;

class ProgressTable : public QTableView
{
    Q_OBJECT
public:
    explicit ProgressTable(QWidget *parent = 0);

    void resizeEvent(QResizeEvent *event);

    void layoutProgressBars();

    void paintEvent(QPaintEvent *e);

protected:
    QList<ProgressBar*> mProgressBars;

signals:
    
public slots:
    void onSectionResized();
};

#endif // PROGRESSTABLE_H
