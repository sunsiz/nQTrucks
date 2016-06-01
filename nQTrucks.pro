TEMPLATE = subdirs
include(common.pri)

#** nQTrucks **#
SUBDIRS += nqtrucks

#** BINS **#
CONFIG   += ordered
SUBDIRS += \
        #testalpr \
        testlib \
        nqtrucksserver

OTHER_FILES += \
            .travis.yml \
            LICENSE \
            README.md
