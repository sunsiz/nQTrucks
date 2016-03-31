QT += script xml sql
greaterThan(QT_MAJOR_VERSION, 4) {
    DEFINES+=HAVE_QT5
    QT+= printsupport widgets
    contains(QT,uitools){
        message(uitools)
        DEFINES += HAVE_UI_LOADER
    }
}

lessThan(QT_MAJOR_VERSION, 5){
    CONFIG(uitools){
        message(uitools)
        DEFINES += HAVE_UI_LOADER
    }
}

NQTRUCKS_VERSION_MAJOR = 1
NQTRUCKS_VERSION_MINOR = 3
NQTRUCKS_VERSION_RELEASE = 10

NQTRUCKS_VERSION = '\\"$${NQTRUCKS_VERSION_MAJOR}.$${NQTRUCKS_VERSION_MINOR}.$${NQTRUCKS_VERSION_RELEASE}\\"'
DEFINES += NQTRUCKS_VERSION_STR=\"$${NQTRUCKS_VERSION}\"
DEFINES += NQTRUCKS_VERSION=$${NQTRUCKS_VERSION}


CONFIG += build_translations
CONFIG += zint
ZINT_PATH = $$PWD/3rdparty/zint-2.4.4

CONFIG(release, debug|release){
    message(Release)
    BUILD_TYPE = release
}else{
    message(Debug)
    BUILD_TYPE = debug
}

BUILD_DIR = $$PWD/build/$${QT_VERSION}
DEST_INCLUDE_DIR = $$PWD/include

unix{
    macx{
        ARCH_DIR       = $${OUT_PWD}/macx
    }
    linux{
        !contains(QT_ARCH, x86_64){
            ARCH_DIR       =  linux32
            message("Compiling for 32bit system")
        }else{
            ARCH_DIR       =  linux64
            message("Compiling for 64bit system")
        }
    }
}
MOC_DIR        = $${ARCH_DIR}/$${BUILD_TYPE}/moc
UI_DIR         = $${ARCH_DIR}/$${BUILD_TYPE}/ui
UI_HEADERS_DIR = $${ARCH_DIR}/$${BUILD_TYPE}/ui
UI_SOURCES_DIR = $${ARCH_DIR}/$${BUILD_TYPE}/ui
OBJECTS_DIR    = $${ARCH_DIR}/$${BUILD_TYPE}/obj
RCC_DIR        = $${ARCH_DIR}/$${BUILD_TYPE}/rcc


#REPORT_PATH = $$PWD/limereport
TRANSLATIONS_PATH = $$PWD/translations

