TEMPLATE = subdirs
include(common.pri)

##** QtZint **##
#SUBDIRS += $$PWD/../../QtZint/QtZint

##** nQAlpr **#
#SUBDIRS += $$PWD/../../nQAlpr/nQAlpr

##** REPORTS  **##
#SUBDIRS += $$PWD/../../nQLReport/nQLReport


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
