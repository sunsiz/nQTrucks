#QT += webkitwidgets
include(../common.pri)
TARGET = testlib
TEMPLATE = app
#CONFIG += shared_and_static build_all

QT += core gui
#CONFIG += console


greaterThan(QT_MAJOR_VERSION, 4){
    QT += widgets printsupport
    DEFINES += HAVE_QT5
}

macx{
    CONFIG  += app_bundle
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += $${INCLUDE_DIR}
DEPENDPATH  += $${INCLUDE_DIR}

#INCLUDEPATH += $${GLOBAL_INCLUDE}/openalpr/include
#DEPENDPATH  += $${GLOBAL_INCLUDE}/openalpr/include

LIBS += -L$${DEST_LIBS}
#-L$${GLOBAL_LIBS}/openalpr/lib
#    CONFIG += link_pkgconfig
#    PKGCONFIG += opencv tesseract
#    LIBS += -lnQTrucks -lopenalpr -lstatedetection -lsupport
#    PRE_TARGETDEPS  += $${DEST_LIBS}/libnQTrucks.a $${GLOBAL_LIBS}/openalpr/lib/libsupport.a $${GLOBAL_LIBS}/openalpr/lib/libsimpleini.a  $${GLOBAL_LIBS}/openalpr/lib/libstatedetection.a

unix:{
    DESTDIR = $${DEST_BINS}
    target.path = $${DEST_BINS}
    INSTALLS = target

    contains(CONFIG,zint){
        LIBS += -lQtZint
    }

    linux{
        #Link share lib to ../lib rpath
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN\/lib
    }
}



RESOURCES += \
        $$PWD/../stylesheets/qdarkstyle/style.qrc
