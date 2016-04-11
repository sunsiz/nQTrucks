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

include(nqtrucks.pri)

LIBS += -L$${DEST_LIBS}


# ***  LIBS **** #
#*** OPENCV  ***#
unix:linux{
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv tesseract
}

#*** nQAlpr ***#
#unix:linux{
#    LIBS += -L$${GLOBAL_LIBS}/openalpr/lib
#    LIBS            += -lopenalpr -lstatedetection -lpthread -lsupport -lsimpleini
#    PRE_TARGETDEPS  +=  $${GLOBAL_LIBS}/openalpr/lib/libsupport.a $${GLOBAL_LIBS}/openalpr/lib/libsimpleini.a  $${GLOBAL_LIBS}/openalpr/lib/libstatedetection.a
#}


EXTRA_FILES += \
    $$PWD/nqtglobal.cpp \
    $$PWD/nqtglobal.h \
    $$PWD/nQTrucksEngine.h
unix:{
    DESTDIR  = $${DEST_LIBS}
    linux{
        QMAKE_POST_LINK += mkdir -p $$quote($${DEST_INCLUDE_DIR}) $$escape_expand(\\n\\t) # qmake need make mkdir -p on subdirs more than root
        for(FILE,EXTRA_FILES){
            QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($${DEST_INCLUDE_DIR}) $$escape_expand(\\n\\t) # copy includes for impl
        }

    }
    #QMAKE_POST_LINK += $(COPY_DIR) $$quote($${DEST_INCLUDE_DIR}) $$quote($${DESTDIR})  #inside of libs make /include/files
}


contains(CONFIG,zint){
    message(zint)
    INCLUDEPATH += $$ZINT_PATH/backend $$ZINT_PATH/backend_qt4
    DEPENDPATH += $$ZINT_PATH/backend $$ZINT_PATH/backend_qt4
    LIBS += -lQtZint
}

####Automatically build required translation files (*.qm)

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




