include(../common.pri)
TARGET = testlib
TEMPLATE = app

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

DEST_DIR       = $${BUILD_DIR}/$${BUILD_TYPE}/$${ARCH_DIR}/$${TARGET}

unix:{
    LIBS += -L$${BUILD_DIR}/$${BUILD_TYPE}/$${ARCH_DIR}/lib -lnQTrucks
    DESTDIR = $$DEST_DIR
    #QMAKE_POST_LINK += mkdir -p $$quote($$REPORTS_DIR) | $$QMAKE_COPY $$quote($$EXTRA_DIR)/* $$quote($$REPORTS_DIR) $$escape_expand(\n\t)

    target.path = $${DEST_DIR}
    INSTALLS = target

    contains(CONFIG,zint){
        LIBS += -L$${BUILD_DIR}/$${BUILD_TYPE}/$${ARCH_DIR}/lib -lQtZint
    }

    linux{
        #Link share lib to ../lib rpath
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/lib
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../lib
        QMAKE_LFLAGS_RPATH += #. .. ./libs
    }
}
