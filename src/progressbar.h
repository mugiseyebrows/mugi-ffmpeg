#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QProgressBar>

class ProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit ProgressBar(QWidget *parent = 0);
    
    void paintEvent(QPaintEvent *);

    void setText(const QString& text);

protected:
    QString mText;

signals:
    
public slots:
    
};

#endif // PROGRESSBAR_H
