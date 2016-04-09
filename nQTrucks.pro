TEMPLATE = subdirs
include(common.pri)
contains(CONFIG, zint){
    SUBDIRS += 3rdparty
}

SUBDIRS += \
        support \
        statedetection \
        openalpr \
        nqtrucks

CONFIG   += ordered

SUBDIRS += \
        testlib
