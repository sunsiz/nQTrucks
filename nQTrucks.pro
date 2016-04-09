TEMPLATE = subdirs
include(common.pri)
contains(CONFIG, zint){
    SUBDIRS += 3rdparty
}

SUBDIRS += \
        statedetection \
        nqtrucks

CONFIG   += ordered

SUBDIRS += \
        testlib
