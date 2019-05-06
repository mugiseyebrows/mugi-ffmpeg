#include "progressbar.h"

#include <QPaintEvent>
#include <QPainter>

ProgressBar::ProgressBar(QWidget *parent) :
    QProgressBar(parent)
{
}

void ProgressBar::paintEvent(QPaintEvent *e)
{
    QProgressBar::paintEvent(e);

    QTextOption opt(Qt::AlignVCenter | Qt::AlignLeft);

    QPainter p(this);
    QString text = p.fontMetrics().elidedText(mText,Qt::ElideRight,e->rect().width() - p.fontMetrics().width(" 100%"));
    p.drawText(e->rect(),text,opt);
}

void ProgressBar::setText(const QString &text)
{
    mText = text;
}
