TARGET = openalpr
TEMPLATE = lib
CONFIG += lib
CONFIG += staticlib
CONFIG += dll
#CONFIG += shared_and_static build_all

CONFIG += create_prl
CONFIG += link_prl


DEFINES += OPENALPR_MAJOR_VERSION=1
DEFINES += OPENALPR_MINOR_VERSION=3
DEFINES += OPENALPR_PATCH_VERSION=10

OPENALPR_VERSION = '\\"$${OPENALPR_MAJOR_VERSION}.$${OPENALPR_MINOR_VERSION}.$${OPENALPR_PATCH_VERSION}\\"'
DEFINES += OPENALPR_VERSION_STR=\"$${OPENALPR}\"
DEFINES += OPENALPR_VERSION=$${OPENALPR}

macx{
    CONFIG  -= dll
    CONFIG  += lib_bundle
    CONFIG  += plugin
}


# add open CV
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    LIBS += libtesseract-dev
    QMAKE_CXXFLAGS += -DOPENCV_MAJOR_VERSION=2
    QMAKE_CXXFLAGS += -DCOMPILE_GPU=1
    #QMAKE_CXXFLAGS += -DDEFAULT_CONFIG_FILE=\\\$\$ORIGIN/../config/openalpr.conf

}

DEFINES += OPENALPR_LIBRARY
RC_FILE = openlapr.rc

EXTRA_FILES += \
    $$PWD/alpr.h

#install (FILES   alpr.h     DESTINATION    ${CMAKE_INSTALL_PREFIX}/include)
#install (TARGETS openalpr-static DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
#install (TARGETS openalpr   DESTINATION    ${CMAKE_INSTALL_PREFIX}/lib)

include(openalpr.pri)

LIBS += -L$${DEST_LIBS}
LIBS += -lsupport
LIBS += -lstatedetection


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




