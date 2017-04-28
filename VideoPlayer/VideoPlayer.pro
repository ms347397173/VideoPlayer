#-------------------------------------------------
#
# Project created by QtCreator 2017-04-27T22:27:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    videoplay.cpp \
    videodecoder.cpp \
    sdldispaly.cpp

HEADERS  += mainwindow.h \
    videoplay.h \
    videodecoder.h \
    sdldispaly.h

FORMS    += mainwindow.ui



#msvc下使用下列语句包含SDL和ffmpeg，同时在项目目录下存在ffmpeg目录和SDL2目录存放include和lib目录

INCLUDEPATH += ../include

 LIBS += -L../lib -lavcodec
 LIBS += -L../lib -lavdevice
 LIBS += -L../lib -lavfilter
 LIBS += -L../lib -lavformat
 LIBS += -L../lib -lavutil
 LIBS += -L../lib -lpostproc
 LIBS += -L../lib -lswscale
 LIBS += -L../lib -lSDL2
 LIBS += -L../lib -lSDL2main
