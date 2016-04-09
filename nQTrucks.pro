TEMPLATE = subdirs
include(common.pri)
contains(CONFIG, zint){
    SUBDIRS += 3rdparty
}

SUBDIRS += \
        nQAlpr \
        nqtrucks

CONFIG   += ordered

SUBDIRS += \
        testlib
