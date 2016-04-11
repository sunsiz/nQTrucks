TEMPLATE = subdirs
include(common.pri)

#** ZINT **#
contains(CONFIG, zint){
    SUBDIRS += 3rdparty
}

#** nQTrucks **#
SUBDIRS += nqtrucks

CONFIG   += ordered


#** BINS **#
SUBDIRS += \
        testlib

OTHER_FILES += \
            .travis.yml \
            LICENSE \
            README.md
