#QT += webkitwidgets
include(../common.pri)
TARGET = testlib
TEMPLATE = app
#CONFIG += shared_and_static build_all

QT += core gui

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

INCLUDEPATH += $$PWD/../include
DEPENDPATH  += $$PWD/../include
##INCLUDEPATH += $$PWD/../nqtrucks
##DEPENDPATH  += $$PWD/../nqtrucks

#DEST_DIR       = $${DEST_BINS}
LIBS += -L$${DEST_LIBS}

unix:{
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv tesseract
    LIBS += -lnQTrucks -lopenalpr -lsupport -lstatedetection
    DESTDIR = $$DEST_BINS
    #QMAKE_POST_LINK += mkdir -p $$quote($$REPORTS_DIR) | $$QMAKE_COPY $$quote($$EXTRA_DIR)/* $$quote($$REPORTS_DIR) $$escape_expand(\n\t)

    target.path = $${DEST_BINS}
    INSTALLS = target

    contains(CONFIG,zint){
        LIBS += -lQtZint
    }

    linux{
        #Link share lib to ../lib rpath
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/lib
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../lib
        QMAKE_LFLAGS_RPATH += #. .. ./libs
    }
}



RESOURCES += \
        $$PWD/../stylesheets/qdarkstyle/style.qrc
