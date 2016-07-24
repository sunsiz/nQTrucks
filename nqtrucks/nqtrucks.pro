include(../common.pri)
QT += network sql core serialport qml script #testlib
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

unix{
    DESTDIR  = $${DEST_LIBS}
    EXTRA_FILES += \
        $$PWD/nqtglobal.cpp \
        $$PWD/nqtglobal.h \
        $$PWD/nQTrucksEngine.h

    EXTRA_FILES2 += \
        $$PWD/Registros/Tools.h \
        $$PWD/Registros/Bascula.h \
        $$PWD/Registros/Camara.h \
        $$PWD/Registros/MatriculaResults.h \
        $$PWD/Registros/RegistroMatriculas.h

    EXTRA_MASCARAS += \
        $$PWD/mascara1.jpg\
        $$PWD/mascara2.jpg

    linux{
        QMAKE_POST_LINK += mkdir -p $$quote($${DEST_INCLUDE_DIR}) $$escape_expand(\\n\\t) # qmake need make mkdir -p on subdirs more than root
        for(FILE,EXTRA_FILES){
            QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($${DEST_INCLUDE_DIR}) $$escape_expand(\\n\\t) # copy includes for impl
        }
        QMAKE_POST_LINK += mkdir -p $$quote($${DEST_INCLUDE_DIR}/Registros) $$escape_expand(\\n\\t) # qmake need make mkdir -p on subdirs more than root
        for(FILE,EXTRA_FILES2){
            QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($${DEST_INCLUDE_DIR}/Registros) $$escape_expand(\\n\\t) # copy includes for impl
        }
        for(FILE,EXTRA_MASCARAS){
            QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($${DEST_BINS}) $$escape_expand(\\n\\t) # copy includes for impl
        }
    }
}

#*** OPENCV  ***#
unix:linux{
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    LIBS += -ltesseract
}

#*** nQAlpr ***#
unix{
    DEPENDPATH  += $${GLOBAL_INCLUDE}/nQAlpr/include
    INCLUDEPATH += $${GLOBAL_INCLUDE}/nQAlpr/include
    INCLUDEPATH += $${SOURCE_GITS}/openalpr/src/openalpr
    DEPENDPATH  += $${SOURCE_GITS}/openalpr/src/openalpr
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv

    LIBS        += -L$${GLOBAL_LIBS}/nQAlpr/lib
    LIBS        += -lnQAlpr -lstatedetection -ltesseract
    PRE_TARGETDEPS  += $${GLOBAL_LIBS}/nQAlpr/lib/libnQAlpr.a  $${GLOBAL_LIBS}/nQAlpr/lib/libstatedetection.a

    EXTRA_DIRS += \
        $${GLOBAL_LIBS}/nQAlpr/config \
        $${GLOBAL_LIBS}/nQAlpr/runtime_data
#        $${GLOBAL_LIBS}/nQAlpr/matriculas
    QMAKE_POST_LINK += $(COPY_DIR) $$quote($${EXTRA_DIRS}) $$quote($${DEST_BINS}) $$escape_expand(\\n\\t) #inside of libs make /include/files

    EXTRA_LIBS += \
        $${GLOBAL_LIBS}/nQAlpr/lib/*
     for(FILES_EXTRA_LIBS,EXTRA_LIBS){
         QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILES_EXTRA_LIBS) $$quote($${DEST_LIBS}) $$escape_expand(\\n\\t) # copy includes for impl
        }

    EXTRA_INCLUDES += \
        $${GLOBAL_INCLUDE}/nQAlpr/include/*
        QMAKE_POST_LINK += mkdir -p $$quote($${DEST_BINS}/include) $$escape_expand(\\n\\t) # qmake need make mkdir -p on subdirs more than root/
     for(FILES_EXTRA_INCLUDES,EXTRA_INCLUDES){
         QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILES_EXTRA_INCLUDES) $$quote($${DEST_BINS}/include/) $$escape_expand(\\n\\t) # copy includes for impl
     }

}

#** LimeReport **#
unix{
    DEPENDPATH  += $${GLOBAL_INCLUDE}/nQLReport/include
    INCLUDEPATH += $${GLOBAL_INCLUDE}/nQLReport/include

    LIBS        += -L$${GLOBAL_LIBS}/nQLReport/lib
    PRE_TARGETDEPS  += $${GLOBAL_LIBS}/nQLReport/lib/liblimereport.a

    EXTRA_LIBS += \
        $${GLOBAL_LIBS}/nQLReport/lib/*
     for(FILES_EXTRA_LIBS,EXTRA_LIBS){
         QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILES_EXTRA_LIBS) $$quote($${DEST_LIBS}) $$escape_expand(\\n\\t) # copy includes for impl
     }

    EXTRA_INCLUDES += \
        $${GLOBAL_INCLUDE}/nQLReport/include/*
        QMAKE_POST_LINK += mkdir -p $$quote($${DEST_BINS}/include) $$escape_expand(\\n\\t) # qmake need make mkdir -p on subdirs more than root/
     for(FILES_EXTRA_INCLUDES,EXTRA_INCLUDES){
         QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILES_EXTRA_INCLUDES) $$quote($${DEST_BINS}/include/) $$escape_expand(\\n\\t) # copy includes for impl
     }
}


# ***nQLReport ***#
unix{
    INCLUDEPATH     += $${DEST_INCLUDE_DIR}
    DEPENDPATH      += $${DEST_INCLUDE_DIR}
    #PRE_TARGETDEPS  += $${DEST_INCLUDE_DIR}
    LIBS += -L$${DEST_LIBS} -llimereport
    LIBS += -L$${DEST_LIBS} -lQtZint
    EXTRA_DIR_REPORTS = $$PWD/reports
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$quote($$EXTRA_DIR_REPORTS) $$quote($$DEST_BINS) $$escape_expand(\n\t)

#    OTHER_FILES += \
#        $$PWD/reports/peso_matricula.lrxml \
#        $$PWD/reports/peso_procesado.lrxml
}



#INSTALLS
linux{
   QMAKE_LFLAGS += -Wl,--rpath=/opt/newsages/lib
    target.path == $${NEWSAGES_LIBS}

    extralibs.path = $${NEWSAGES_LIBS}
    extralibs.files = $${EXTRA_LIBS}

    extradirs.path = $${NEWSAGES_DIR}/nQTrucks
    extradirs.files = $${EXTRA_DIRS}

    reports.path = $${NEWSAGES_DIR}/nQTrucks
    reports.files = $${EXTRA_DIR_REPORTS}

    INSTALLS += target extradirs extralibs reports
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




