include(../common.pri)
QT += network sql core serialport qml #testlib
TARGET = nQTrucks
TEMPLATE = lib
#CONFIG += lib
#CONFIG += staticlib
#CONFIG += dll
CONFIG += shared_and_static build_all

CONFIG += create_prl
CONFIG += link_prl

macx{
    CONFIG  -= dll
    CONFIG  += lib_bundle
    CONFIG  += plugin
}

DEFINES += NQTRUCKS_LIBRARY
RC_FILE = nQTrucks.rc

#** LIBS  **#
LIBS += -L$${DEST_LIBS}

unix:{
    DESTDIR  = $${DEST_LIBS}
    EXTRA_FILES += \
        $$PWD/nqtglobal.cpp \
        $$PWD/nqtglobal.h \
        $$PWD/nQTrucksEngine.h
    linux{
        QMAKE_POST_LINK += mkdir -p $$quote($${DEST_INCLUDE_DIR}) $$escape_expand(\\n\\t) # qmake need make mkdir -p on subdirs more than root
        for(FILE,EXTRA_FILES){
            QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($${DEST_INCLUDE_DIR}) $$escape_expand(\\n\\t) # copy includes for impl
        }
    }
}

#*** OPENCV  ***#
unix:linux{
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    LIBS += -ltesseract
}
#** ZINT **#
#CONFIG += zint
#ZINT_PATH = $$PWD/3rdparty/zint-2.4.4
contains(CONFIG,zint){
    message(zint)
    INCLUDEPATH += $$ZINT_PATH/backend $$ZINT_PATH/backend_qt4
    DEPENDPATH += $$ZINT_PATH/backend $$ZINT_PATH/backend_qt4
    LIBS += -lQtZint
    PRE_TARGETDEPS  += $${DEST_LIBS}/libQtZint.a
}

#*** nQAlpr ***#
unix:{
    DEPENDPATH  += $${GLOBAL_INCLUDE}/openalpr/include
    INCLUDEPATH += $${GLOBAL_INCLUDE}/openalpr/include
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv

    LIBS        += -L$${GLOBAL_LIBS}/openalpr/lib
    LIBS +=  -lopenalpr -lstatedetection -ltesseract
    PRE_TARGETDEPS  += $${GLOBAL_LIBS}/openalpr/lib/libopenalpr.a  $${GLOBAL_LIBS}/openalpr/lib/libstatedetection.a

    EXTRA_DIRS += \
        $${GLOBAL_LIBS}/openalpr/config \
        $${GLOBAL_LIBS}/openalpr/runtime_data \
        $${GLOBAL_LIBS}/openalpr/matriculas
    QMAKE_POST_LINK += $(COPY_DIR) $$quote($${EXTRA_DIRS}) $$quote($${DEST_BINS}) $$escape_expand(\\n\\t) #inside of libs make /include/files

    EXTRA_LIBS += \
        $${GLOBAL_LIBS}/openalpr/lib/*
     for(FILES_EXTRA_LIBS,EXTRA_LIBS){
         QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILES_EXTRA_LIBS) $$quote($${DEST_LIBS}) $$escape_expand(\\n\\t) # copy includes for impl
        }
}


include(nqtrucks.pri)

####Automatically build required translation files (*.qm)
#REPORT_PATH = $$PWD/limereport
CONFIG += build_translations
TRANSLATIONS_PATH = $$PWD/../translations
contains(CONFIG,build_translations){
    LANGUAGES = ru es_ES

    defineReplace(prependAll) {
        for(a,$$1):result += $$2$${a}$$3
        return($$result)
    }

    TRANSLATIONS = $$prependAll(LANGUAGES, $$TRANSLATIONS_PATH/nqtrucks_,.ts)

    qtPrepareTool(LUPDATE, lupdate)
    ts.commands = $$LUPDATE $$PWD -ts $$TRANSLATIONS

    TRANSLATIONS_FILES =
    qtPrepareTool(LRELEASE, lrelease)
    for(tsfile, TRANSLATIONS) {
        qmfile = $$tsfile
        qmfile ~= s,.ts$,.qm,
        qm.commands += $$LRELEASE -removeidentical $$tsfile -qm $$qmfile $$escape_expand(\\n\\t)
        tmp_command = $$LRELEASE -removeidentical $$tsfile -qm $$qmfile $$escape_expand(\\n\\t)
        TRANSLATIONS_FILES += $$qmfile
    }
    qm.depends = ts
    OTHER_FILES += $${TRANSLATIONS}
    QMAKE_EXTRA_TARGETS += qm ts
    POST_TARGETDEPS +=  qm
}

#### EN AUTOMATIC TRANSLATIONS




