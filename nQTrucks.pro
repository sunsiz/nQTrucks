TEMPLATE = subdirs
include(common.pri)

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
