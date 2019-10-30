#ifndef SOURCESMODEL_H
#define SOURCESMODEL_H

#include <QStandardItemModel>
#include <QMap>

class SourcesModel : public QStandardItemModel
{
    Q_OBJECT
public:

    enum Type {
        Video,
        Audio,
        Subtitles
    };
    enum Columns {
        ColumnType,
        ColumnPath
    };

    explicit SourcesModel(QObject *parent = 0);

    void addVideo(const QString& path);
    void addAudio(const QString& path);
    void addSubtitles(const QString& path);

    void addSource(Type type, const QString& path);

    QStringList files() const;

    QList<QStringList> tasks(const QList<int> &checked,
                             const QStringList& options,
                             const QString& output) const;

    void appendRow(Type type, const QString &path);
    void appendRow(Type type, const QString &path, const QStringList &files);

    static QStringList findFiles(const QString &path, const QStringList &exts);
    static QStringList findFiles(const QString& path, Type type);
    //static QMap<QString, QString> mapBaseNamePath(const QStringList &files);

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    void removeAll();

    Type type(int row);



protected:

    QList<QStringList> mFiles;
    QList<QMap<int,int> > mMapping;
    QList<int> mIndexes;

    void updateMapping();
    void updateIndexes();
signals:
    void sourceRemoved();

public slots:

};

#endif // SOURCESMODEL_H
