#ifndef OPTIONS_H
#define OPTIONS_H

class QLineEdit;
class SourcesModel;
#include <QMap>
#include <QObject>

class Options : public QObject
{
    Q_OBJECT
public:
    enum OptionType {
        VideoType,
        AudioType,
        OtherType
    };

    Options(QObject* parent = 0);

    ~Options() override;

    void lineEdit(OptionType type, QLineEdit* edit);

    void remove(OptionType type, const QString& name);
    void add(OptionType type, const QString& name, const QString& value);

    void syncMaps(SourcesModel *sourcesModel);

public slots:
    void onVideoCopy();
    void onX264();
    void onNoScale();
    void onScale720();
    void onVorbis();
    void onAudioCopy();
protected:
    QMap<OptionType,QLineEdit*> mOptionLineEdit;


};

#endif // OPTIONS_H
