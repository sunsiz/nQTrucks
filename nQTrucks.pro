TEMPLATE = subdirs
include(common.pri)

#** ZINT **#
contains(CONFIG, zint){
    SUBDIRS += 3rdparty
}

##** nQAlpr **#
#SUBDIRS += $$PWD/../../nQAlpr/nQAlpr

#** nQTrucks **#
SUBDIRS += nqtrucks

#** BINS **#
CONFIG   += ordered
SUBDIRS += \
        #testalpr \
        testlib

OTHER_FILES += \
            .travis.yml \
            LICENSE \
            README.md
