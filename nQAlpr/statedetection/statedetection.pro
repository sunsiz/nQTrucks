TARGET = statedetection
TEMPLATE = lib
CONFIG += lib
CONFIG += staticlib
CONFIG += dll
#CONFIG += shared_and_static build_all

CONFIG += create_prl
CONFIG += link_prl



macx{
    CONFIG  -= dll
    CONFIG  += lib_bundle
    CONFIG  += plugin
}
OPENCV_MAJOR_VERSION = 2
# add open CV
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    CONFIG += OPENCV_MAJOR_VERSION=2
}

INCLUDEPATH += $$PWD/../support
DEPENDPATH  += $$PWD/../support

#DEFINES += NQTRUCKS_LIBRARY
#RC_FILE = nQTrucks.rc

EXTRA_FILES += \
    $$PWD/state_detector.h

include(statedetection.pri)
LIBS += -L$${DEST_LIBS}
unix:{
    DESTDIR  = $${DEST_LIBS}
    linux{
        QMAKE_POST_LINK += mkdir -p $$quote($${DEST_INCLUDE_DIR}) $$escape_expand(\\n\\t) # qmake need make mkdir -p on subdirs more than root
        for(FILE,EXTRA_FILES){
            QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($${DEST_INCLUDE_DIR}) $$escape_expand(\\n\\t) # copy includes for impl
        }

    }
    QMAKE_POST_LINK += $(COPY_DIR) $$quote($${DEST_INCLUDE_DIR}) $$quote($${DESTDIR})  #inside of libs make /include/files
}




