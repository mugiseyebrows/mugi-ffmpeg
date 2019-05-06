#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
public:

    static Settings* instance();

    QString settingsPath() const;
    QString source() const;
    QString output() const;
    bool overwrite() const;
    void save();
    void setSource(const QString &value);
    void setOutput(const QString &value);
    void setOverwrite(bool value);
protected:
    static Settings* mInstance;
    QString mDir;
    Settings();

    QString mSource;
    QString mOutput;
    bool mOverwrite;


};

#endif // SETTINGS_H
