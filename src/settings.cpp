#include "settings.h"

#include <QStandardPaths>
#include <QDir>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "jsonhelper.h"

Settings* Settings::mInstance = 0;

Settings* Settings::instance() {
    if (!mInstance) {
        mInstance = new Settings();
    }
    return mInstance;
}

Settings::Settings() {
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString name = qApp->applicationName();
    QDir d(appData);
    if (!d.exists()) {
        d.cdUp();
        if (!d.mkdir(name)) {
            QString error = QString("Can not create directory %1").arg(appData);
            QMessageBox::critical(qApp->activeWindow(),"Error",error);
        }
        d.cd(name);
    }
    mDir = appData;


    QString path = settingsPath();

    if (!QFile::exists(path)) {
        return;
    }

    bool ok;

    QJsonDocument doc = loadJson(path,&ok);

    if (!ok) {
        QMessageBox::critical(qApp->activeWindow(),"Error",QString("Failed to load settings %1").arg(path));
        return;
    }

    QJsonObject obj = doc.object();

    mSource = obj.value("source").toString();
    mOutput = obj.value("output").toString();
    mOverwrite = obj.value("overwrite").toBool();
}

QString Settings::source() const {
    return mSource;
}

QString Settings::output() const {
    return mOutput;
}

bool Settings::overwrite() const {
    return mOverwrite;
}

void Settings::setSource(const QString& value) {
    mSource = value;
}

void Settings::setOutput(const QString& value) {
    mOutput = value;
}

void Settings::setOverwrite(bool value) {
    mOverwrite = value;
}

void Settings::save() {

    QJsonObject settings;
    settings["source"] = mSource;
    settings["output"] = mOutput;
    settings["overwrite"] = mOverwrite;
    saveJson(settingsPath(),settings);
}

QString Settings::settingsPath() const
{
    return QDir(mDir).filePath("settings.json");
}

