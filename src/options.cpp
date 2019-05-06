#include "options.h"

#include <QLineEdit>
#include "sourcesmodel.h"
#include <QSet>

void Options::remove(Options::OptionType type, const QString &name)
{
    QLineEdit* lineEdit = mOptionLineEdit[type];
    QStringList options = lineEdit->text().split(" ");
    QSet<int> skip;
    for(int i=0;i<options.size();i++) {
        if (options[i] == name || options[i].isEmpty()) {
            skip << i;
            if (options[i].startsWith("-") && i+1 < options.size())
                skip << i+1;
        }
    }
    QStringList options_;
    for(int i=0;i<options.size();i++) {
        if (!skip.contains(i))
            options_ << options[i];
    }
    lineEdit->setText(options_.join(" ").trimmed());
}

void Options::add(Options::OptionType type, const QString &name, const QString &value)
{
    QLineEdit* lineEdit = mOptionLineEdit[type];
    QString text = lineEdit->text();
    text = text + " " + name;
    if (!value.isEmpty())
        text = text + " " + value;
    lineEdit->setText(text.trimmed());
}

void Options::syncMaps(SourcesModel* sourcesModel)
{
    OptionType t = OtherType;

    remove(t,"-map");

    QMap<SourcesModel::Type, QString> ts;
    ts[SourcesModel::Video] = "v";
    ts[SourcesModel::Audio] = "a";
    ts[SourcesModel::Subtitles] = "s";

    int video = 0;
    int audio = 0;

    for(int i=0;i<sourcesModel->rowCount();i++) {
        SourcesModel::Type st = sourcesModel->type(i);
        add(t,"-map",QString("%1:%2").arg(i).arg(ts[st]));
        if (st == SourcesModel::Video)
            video++;
        if (st == SourcesModel::Audio)
            audio++;
    }

}

Options::Options(QObject *parent) : QObject(parent)
{

}

Options::~Options()
{

}

void Options::lineEdit(Options::OptionType type, QLineEdit *edit)
{
    mOptionLineEdit[type] = edit;
}

void Options::onVideoCopy()
{
    OptionType t = VideoType;
    remove(t,"-c:v");
    remove(t,"-crf");
    remove(t,"-preset");
    add(t,"-c:v","copy");
}

void Options::onX264()
{
    OptionType t = VideoType;
    remove(t,"-c:v");
    remove(t,"-crf");
    remove(t,"-preset");
    add(t,"-c:v","libx264");
    add(t,"-crf","18");
    add(t,"-preset","fast");
}

void Options::onScale720() {
    OptionType t = VideoType;
    remove(t,"-vf");
    add(t,"-vf","scale=-1:720");
}

void Options::onNoScale() {
    OptionType t = VideoType;
    remove(t,"-vf");
}

void Options::onAudioCopy() {
    OptionType t = AudioType;
    remove(t,"-c:a");
    remove(t,"-aq");
    add(t,"-c:a","copy");
}

void Options::onVorbis()
{
    OptionType t = AudioType;
    remove(t,"-c:a");
    remove(t,"-aq");
    add(t,"-c:a","libvorbis");
    add(t,"-aq","5");
}
