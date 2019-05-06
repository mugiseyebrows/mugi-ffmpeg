#ifndef OUTPUTPARSER_H
#define OUTPUTPARSER_H

#include <QObject>

class OutputParser : public QObject
{
    Q_OBJECT
public:
    explicit OutputParser(QObject *parent = 0);
    
    void clear();

    void parse(const QString& text);

    int duration() const;
    int time() const;
    double speed() const;

    int parseToSeconds(const QStringList &caps);
protected:
    int mDuration;
    int mTime;
    double mSpeed;

signals:
    
public slots:
    
};

#endif // OUTPUTPARSER_H
