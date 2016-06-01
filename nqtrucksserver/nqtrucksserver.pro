include(../common.pri)
#QT += webkitwidgets
TARGET = nqtrucksserver
TEMPLATE = app
QT += core gui dbus
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
    INCLUDEPATH += $${GLOBAL_INCLUDE}
    INCLUDEPATH += $${SOURCE_GITS}/openalpr/src/openalpr
    DEPENDPATH  += $${SOURCE_GITS}/openalpr/src/openalpr
    INCLUDEPATH += $${INCLUDE_DIR}
    DEPENDPATH  += $${INCLUDE_DIR}
    LIBS += -lnQTrucks
    linux{
        #Link share lib to ../lib rpath
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN\/lib
        QMAKE_LFLAGS += -Wl,--rpath=/opt/newsages/lib
        target.path = $${NEWSAGES_DIR}/nQTrucks
        INSTALLS = target
    }
}



include(Desktop/Desktop.pri)
include(Configuracion/Configuracion.pri)

SOURCES += main.cpp

RESOURCES += \
    $$PWD/../resources/system.qrc \
    $$PWD/../stylesheets/qdarkstyle/style.qrc

