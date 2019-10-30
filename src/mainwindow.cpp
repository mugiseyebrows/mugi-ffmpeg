#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include "selector.h"

#include <QDesktopServices>
#include <QUrl>

#include <QStandardItemModel>
#include <QListWidgetItem>
#include <QTimer>
#include "tasksmodel.h"
#include "worker.h"
#include "outputparser.h"

#include "sourcesmodel.h"
#include "deletefilter.h"

#include "taskitemdelegate.h"

#include "arrangefilter.h"

#include "sourceitemdelegate.h"

#include <QMessageBox>
#include "jsonhelper.h"
#include "settings.h"


#if 0
#include "automate_p.cpp"
#else
void automate(QWidget*) {

}
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mWorker(new Worker(this)), mOutputParser(new OutputParser(this)), mQuit(false), mStartNext(true), mOptions(this)
{
    ui->setupUi(this);

    TasksModel* tasksModel = new TasksModel(this);

    setWindowTitle(QString("%1 %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));

    mOptions.lineEdit(Options::VideoType, ui->optionsVideo);
    mOptions.lineEdit(Options::AudioType, ui->optionsAudio);
    mOptions.lineEdit(Options::OtherType, ui->optionsOther);

    ui->tasks->setModel(tasksModel);

    SourcesModel* sourcesModel = new SourcesModel(ui->sources);
    ui->sources->setModel(sourcesModel);
    connect(sourcesModel,SIGNAL(sourceRemoved()),this,SLOT(onSourceRemoved()));

    connect(this,SIGNAL(nextTask(int,QStringList,bool)),mWorker,SLOT(onNextTask(int,QStringList,bool)));
    connect(mWorker,SIGNAL(finished()),this,SLOT(onTaskFinished()));
    connect(mWorker,SIGNAL(stdErrData(QString)),this,SLOT(onStdErrData(QString)));

    DeleteFilter* delete1 = new DeleteFilter();
    delete1->setView(ui->sources);
    delete1->setAction(DeleteFilter::RemoveRow);

    ui->tasks->setItemDelegate(new TaskItemDelegate(ui->tasks));

    connect(tasksModel,SIGNAL(probe(QString)),mWorker,SLOT(onProbe(QString)));
    connect(mWorker,SIGNAL(probed(QString,double)),tasksModel,SLOT(onProbed(QString,double)));

    ui->progressTotal->setMaximum(1000);

    connect(this,SIGNAL(terminate()),mWorker,SLOT(onTerminate()));
    connect(mWorker,SIGNAL(terminated()),this,SLOT(onTerminated()));

    ArrangeFilter* arrange = new ArrangeFilter();
    arrange->setView(ui->tasks);
    arrange->setRoles({Qt::EditRole, TasksModel::Id, TasksModel::Progress, TasksModel::Duration});
    arrange->setEnabled(true);

    DeleteFilter* delete2 = new DeleteFilter(ui->tasks);
    delete2->setView(ui->tasks);
    delete2->setAction(DeleteFilter::RemoveRow);

    ui->sources->setItemDelegateForColumn(SourcesModel::ColumnType,new SourceItemDelegate(ui->sources));

    loadSettings();

    connect(ui->videoCopy,SIGNAL(clicked()),&mOptions,SLOT(onVideoCopy()));
    connect(ui->x264,SIGNAL(clicked()),&mOptions,SLOT(onX264()));
    connect(ui->scale720,SIGNAL(clicked()),&mOptions,SLOT(onScale720()));
    connect(ui->noScale,SIGNAL(clicked()),&mOptions,SLOT(onNoScale()));
    connect(ui->audioCopy,SIGNAL(clicked()),&mOptions,SLOT(onAudioCopy()));
    connect(ui->vorbis,SIGNAL(clicked()),&mOptions,SLOT(onVorbis()));

    automate(this);
}

void MainWindow::onProbed(QString, double)
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProgressTotal() {

    ui->progressTotal->setValue( (int)(tasksModel()->totalProgress() * 1000.0) );
}

void MainWindow::onTaskFinished() {

    if (mQuit || !mStartNext)
        return;

    updateProgressTotal();

    TasksModel* tasksModel = this->tasksModel();

    if (mTask > -1) {
        tasksModel->setProgress(mTask,1.0);
    }

    mTask = tasksModel->nextTask();
    if (mTask < 0) {
        return;
    }

    QModelIndex idx = tasksModel->findTask(mTask);

    QStringList args = tasksModel->data(idx).toStringList();

    emit nextTask(mTask, args, ui->overwrite->isChecked());
}

void MainWindow::onStdErrData(QString text) {
    ui->log->appendPlainText(text);
    mOutputParser->parse(text);

    int duration = mOutputParser->duration();
    int time = mOutputParser->time();

    TasksModel* tasksModel = this->tasksModel();

    if (duration >= 0 && time >= 0 && mTask > -1) {
        tasksModel->setProgress(mTask, (double) time / duration );
    }

    if (mEstimator.needData()) {
        mEstimator.setData(tasksModel->totalDuration(),tasksModel->completeDuration());
    }
    ui->time->setText(QString("Time %1 estimated %2").arg(mEstimator.time()).arg(mEstimator.estimated()));
    updateProgressTotal();
}

void MainWindow::onTerminated()
{
    qDebug() << "on terminated mQuit" << mQuit;
    if (mQuit) {
        qDebug() << "call close";
        QTimer::singleShot(0,this,SLOT(close()));
    }
}

void MainWindow::selectDirectory(QLineEdit* edit) {
    QString path = edit->text();
    path = QFileDialog::getExistingDirectory(this,QString(),path);
    if (!path.isEmpty())
        edit->setText(path);
}

void MainWindow::setListWidgetItems(QListWidget* widget, const QStringList& items) {
    ui->files->clear();
    QString name;

    foreach(name,items) {
        QListWidgetItem* item = new QListWidgetItem(widget);
        item->setText( name );
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

QStringList nonEmpty(const QStringList& items) {
    QStringList res;
    QString item;
    foreach(item,items) {
        if (!item.isEmpty())
            res << item;
    }
    return res;
}

void MainWindow::on_enqueue_clicked() {

    SourcesModel* sourcesModel = this->sourcesModel();

    if (sourcesModel->rowCount() < 1) {
        QMessageBox::critical(this, "Error", "Specify at least one source (video or audio)");
        return;
    }

    QStringList options;
    options.append( ui->optionsAudio->text().split(" ") );
    options.append( ui->optionsVideo->text().split(" ") );
    options.append( ui->optionsOther->text().split(" ") );

    options = nonEmpty(options);

    QList<int> checked;

    for (int i=0;i<ui->files->count();i++) {
        QListWidgetItem* item = ui->files->item(i);
        if (item->checkState() == Qt::Checked)
            checked << i;
    }

    if (checked.isEmpty()) {
        QMessageBox::critical(this, "Error", "Select files");
        return;
    }

    QString output = ui->output->text();
    if (output.isEmpty()) {
        QMessageBox::critical(this, "Error", "Output path not specified");
        return;
    }

    if (!output.contains("%name%")) {
        output = QDir(output).filePath("%name%.mkv");
    }

    tasksModel()->append(sourcesModel->tasks(checked,options,output));
}

void MainWindow::on_run_clicked() {
    mEstimator.clear();
    mTask = -1;
    mStartNext = true;
    onTaskFinished();
    mTime = QTime::currentTime();
}

void MainWindow::on_selectOutputDirectory_clicked() {
    selectDirectory(ui->output);

    QString output = ui->output->text();
    ui->output->setText(QDir::toNativeSeparators(QDir(output).filePath("%name%.mkv")));

}

void MainWindow::on_addVideo_clicked()
{
    onAddSource(SourcesModel::Video);
}

void MainWindow::on_addAudio_clicked()
{
    onAddSource(SourcesModel::Audio);
}

void MainWindow::on_addSubtitles_clicked()
{
    onAddSource(SourcesModel::Subtitles);
}

void MainWindow::onAddSource(SourcesModel::Type type)
{
    SourcesModel* sourcesModel = this->sourcesModel();
    QString path = QFileDialog::getExistingDirectory(this,QString(),mPath);
    if (path.isEmpty()) {
        return;
    }
    mPath = path;
    sourcesModel->addSource(type, path);
    setListWidgetItems(ui->files,sourcesModel->files());
    mOptions.syncMaps(sourcesModel);
}

void MainWindow::setCheckState(QListWidget* widget, Qt::CheckState state) {
    for(int i=0;i<widget->count();i++) {
        widget->item(i)->setCheckState(state);
    }
}

void MainWindow::loadSettings()
{    
    Settings* settings = Settings::instance();
    mPath = settings->source();
    ui->output->setText(settings->output());
    ui->overwrite->setChecked(settings->overwrite());
}

void MainWindow::saveSettings()
{
    Settings* settings = Settings::instance();
    settings->setSource(mPath);
    settings->setOutput(ui->output->text());
    settings->setOverwrite(ui->overwrite->isChecked());
    settings->save();
}

TasksModel *MainWindow::tasksModel()
{
    return qobject_cast<TasksModel*>(ui->tasks->model());
}

SourcesModel *MainWindow::sourcesModel()
{
    return qobject_cast<SourcesModel*>(ui->sources->model());
}

void MainWindow::closeEvent(QCloseEvent * e)
{
    qDebug() << "close event mQuit" << mQuit;
    if (mWorker->isRunning()) {
        qDebug() << "worker is running";
        if (QMessageBox::question(this,"ffmpeg is running","Stop ffmpeg and exit?",QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            mQuit = true;
            emit terminate();
        }
        qDebug() << "ignore event";
        e->ignore();
        return;
    }
    qDebug() << "accept event";

    saveSettings();

    e->accept();
}

void MainWindow::on_selectAllFiles_clicked() {
    setCheckState(ui->files,Qt::Checked);
}

void MainWindow::on_unselectAllFiles_clicked() {
    setCheckState(ui->files,Qt::Unchecked);
}

void MainWindow::onSourceRemoved() {
    SourcesModel* sourcesModel = this->sourcesModel();
    setListWidgetItems(ui->files,sourcesModel->files());
    mOptions.syncMaps(sourcesModel);
}

void MainWindow::on_stopAfterCurrent_clicked() {
    mStartNext = false;
}

void MainWindow::on_abort_clicked() {
    mStartNext = false;
    mTask = -1;
    emit terminate();
}

void MainWindow::on_deleteAllTasks_clicked() {
    tasksModel()->removeAll();
}

void MainWindow::on_deleteCompleteTasks_clicked() {
    tasksModel()->removeComplete();
}

void MainWindow::on_deleteAllSources_clicked()
{
    sourcesModel()->removeAll();
}
