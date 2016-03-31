TEMPLATE = subdirs
include(common.pri)
contains(CONFIG, zint){
    SUBDIRS += 3rdparty
}

SUBDIRS += \
        nqtrucks

CONFIG   += ordered
