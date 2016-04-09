TARGET = video
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

DEFINES += VIDEO_LIBRARY
RC_FILE = video.rc

include(video.pri)

LIBS += -L$${DEST_LIBS}
LIBS += -lsupport

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




