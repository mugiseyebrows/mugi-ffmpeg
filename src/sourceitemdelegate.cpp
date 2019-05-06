#include "sourceitemdelegate.h"

#include "sourcesmodel.h"

SourceItemDelegate::SourceItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QString SourceItemDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    int v = value.toInt();

    if (v == SourcesModel::Video) {
        return "Video";
    } else if (v == SourcesModel::Audio) {
        return "Audio";
    } else if (v == SourcesModel::Subtitles) {
        return "Subtitles";
    }
    return QString();
}
