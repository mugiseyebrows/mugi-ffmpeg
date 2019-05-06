#include "progresstable.h"

#include <QDebug>
#include <QHeaderView>
#include <QProgressBar>
#include <QResizeEvent>
#include "progressbar.h"

ProgressTable::ProgressTable(QWidget *parent) :
    QTableView(parent)
{
    connect(horizontalHeader(),SIGNAL(sectionResized(int,int,int)),this, SLOT(onSectionResized()));
    connect(verticalHeader(),SIGNAL(sectionResized(int,int,int)),this, SLOT(onSectionResized()));
}

void ProgressTable::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    layoutProgressBars();

}

void ProgressTable::layoutProgressBars()
{
    QHeaderView* hh = horizontalHeader();
    QHeaderView* vh = verticalHeader();
    int hs = 0;
    for(int i=0;i<vh->count();i++) {
        if (mProgressBars.size() <= i)
            mProgressBars.append(new ProgressBar(viewport()));
        ProgressBar* bar = mProgressBars[i];
        bar->show();

        //qDebug() << bar->sizeHint();
        //qDebug() << bar->minimumSizeHint();

        int h = vh->sectionSize(i);
        int w = hh->sectionSize(0);

        bar->setGeometry(QRect(QPoint(0,hs),QSize(w,h)));

        bar->setValue(i * 20);
        //bar->setTextVisible(false);

        if (i == 0) {
            bar->setValue(100);
        }

        if (model()) {
            QAbstractItemModel* m = model();
            bar->setText(m->data(m->index(i,0)).toString());
        }

        hs += h;
    }

}

#include <QPainter>

void ProgressTable::paintEvent(QPaintEvent *e)
{
    QTableView::paintEvent(e);

    if (!model())
        return;

    int gridSize = 0;


    int colp = columnViewportPosition(0);
    int colw = columnWidth(0) - gridSize;



    bool showGrid = this->showGrid();
    int rightToLeft = isRightToLeft();
    int rowCount = model()->rowCount();
    for(int i=0;i<rowCount;i++) {

        int rowY = rowViewportPosition(i);
        int rowh = rowHeight(i) - gridSize;

        QRect rect(colp + (showGrid && rightToLeft ? 1 : 0), rowY, colw, rowh);
        QPainter p(viewport());
    }

}

void ProgressTable::onSectionResized() {
    layoutProgressBars();
}
