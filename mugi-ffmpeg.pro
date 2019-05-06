
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mugi-ffmpeg
TEMPLATE = app

INCLUDEPATH += src

HEADERS += \
    src/action.h \
    src/arrangefilter.h \
    src/automation.h \
    src/deletefilter.h \
    src/estimator.h \
    src/fileext.h \
    src/jsonhelper.h \
    src/mainwindow.h \
    src/options.h \
    src/outputparser.h \
    src/progressbar.h \
    src/progresstable.h \
    src/selector.h \
    src/settings.h \
    src/sourceitemdelegate.h \
    src/sourcesmodel.h \
    src/taskitemdelegate.h \
    src/tasksmodel.h \
    src/version.h \
    src/worker.h \
    src/lit.h

SOURCES += \
    src/action.cpp \
    src/arrangefilter.cpp \
    src/automation.cpp \
    src/deletefilter.cpp \
    src/estimator.cpp \
    src/fileext.cpp \
    src/jsonhelper.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/options.cpp \
    src/outputparser.cpp \
    src/progressbar.cpp \
    src/progresstable.cpp \
    src/selector.cpp \
    src/settings.cpp \
    src/sourceitemdelegate.cpp \
    src/sourcesmodel.cpp \
    src/taskitemdelegate.cpp \
    src/tasksmodel.cpp \
    src/worker.cpp \
    src/lit.cpp

FORMS += \
    src/mainwindow.ui

RESOURCES += \
    src/resources.qrc

RC_FILE = mugi-ffmpeg.rc

DISTFILES += \
    todo.txt

target.path = /usr/local/bin
INSTALLS += target
