#-------------------------------------------------
#
# Project created by QtCreator 2015-06-28T21:35:26
#
#-------------------------------------------------

QT       += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 5):

TARGET = LibraryProject
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tree.cpp \
    pictureflow.cpp \
    columneditdialog.cpp \
    getmoviedata.cpp \
    getmoviedatapromptdialog.cpp \
    sidebar.cpp \
    castandcrewlinks.cpp \
    getseriesdataprompt.cpp

HEADERS  += mainwindow.h \
    tree.h \
    pictureflow.h \
    columneditdialog.h \
    getmoviedata.h \
    getmoviedatapromptdialog.h \
    sidebar.h \
    castandcrewlinks.h \
    getseriesdataprompt.h

RESOURCES += \
    resources.qrc

FORMS += \
    Options.ui \
    GetSeriesName.ui
