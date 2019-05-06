#include "taskitemdelegate.h"

#include "tasksmodel.h"
#include <QDebug>

#include <QPainter>
#include <QApplication>
#include <QStyle>
#include <QProgressBar>
#include <QTableView>

TaskItemDelegate::TaskItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

QString TaskItemDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if (value.type() == QVariant::StringList) {
        QStringList sl = value.toStringList();
        QStringList q;
        QString s;
        foreach(s,sl) {
            if (s.indexOf(" ") > -1)
                q << ("\"" + s + "\"");
            else
                q << s;
        }
        return q.join(" ");
    }
    return QString();
}

//#include <QStylePainter>
//#include <QTime>

void TaskItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{


    double v = index.data(TasksModel::Progress).toDouble();

    //QStyle* style = qApp->style();

    //option.palette.color()

    QStyleOptionProgressBarV2 opt;
    opt.state |= QStyle::State_Horizontal;
    opt.minimum = 0;
    opt.maximum = 1000;
    opt.progress = (int)(v * 1000.0);
    opt.rect = option.rect;

    QStyle* style = qApp->style();
    style->drawControl(QStyle::CE_ProgressBarContents, &opt, painter);

    QStyledItemDelegate::paint(painter,option,index);

}
