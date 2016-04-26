include(../common.pri)
#QT += webkitwidgets
TARGET = testlib
TEMPLATE = app
QT += core gui
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4){
    QT += widgets printsupport
    DEFINES += HAVE_QT5
}

# *** LIBS ***#
# ** nQTrucks ** #
unix:{
    DESTDIR = $$DEST_BINS
    LIBS += -L$${DEST_LIBS}
    INCLUDEPATH += $${SOURCE_GITS}/openalpr/src/openalpr
    DEPENDPATH  += $${SOURCE_GITS}/openalpr/src/openalpr
    INCLUDEPATH += $${INCLUDE_DIR}
    DEPENDPATH  += $${INCLUDE_DIR}
    LIBS += -lnQTrucks
    linux{
        #Link share lib to ../lib rpath
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN\/lib
    }
    target.path = $${DEST_BINS}
    INSTALLS = target
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
        $$PWD/../stylesheets/qdarkstyle/style.qrc
