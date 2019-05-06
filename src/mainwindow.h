#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

class QLineEdit;
class Selector;
class TasksModel;
class Worker;
class OutputParser;
class QListWidget;

#include <QTime>
#include "options.h"

#include "sourcesmodel.h"
#include "estimator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:


    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:

    void selectDirectory(QLineEdit *edit);

    void setListWidgetItems(QListWidget *widget, const QStringList &items);

    void updateProgressTotal();

    void setCheckState(QListWidget *widget, Qt::CheckState state);

    QString settingsPath() const;

    void closeEvent(QCloseEvent *);

    void loadSettings();

    void saveSettings();

    TasksModel* tasksModel();

    SourcesModel* sourcesModel();

    Worker* mWorker;
    OutputParser* mOutputParser;

    QString mPath;

    int mTask;

    QTime mTime;

    bool mQuit;
    bool mStartNext;

    Estimator mEstimator;

    Options mOptions;

signals:

    void nextTask(int,QStringList, bool);

    void probe(QString);

    void terminate();

protected slots:



    void onProbed(QString,double);

    void onTaskFinished();
    void onStdErrData(QString text);

    void onTerminated();

    void on_selectOutputDirectory_clicked();

    void on_enqueue_clicked();

    void on_run_clicked();

    void on_addVideo_clicked();
    void on_addAudio_clicked();
    void on_addSubtitles_clicked();

    void onAddSource(SourcesModel::Type);

    void on_selectAllFiles_clicked();
    void on_unselectAllFiles_clicked();
    void onSourceRemoved();

    void on_stopAfterCurrent_clicked();
    void on_abort_clicked();
    void on_deleteAllTasks_clicked();
    void on_deleteCompleteTasks_clicked();

    void on_deleteAllSources_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
