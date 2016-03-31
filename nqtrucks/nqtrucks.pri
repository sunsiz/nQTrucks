include(../common.pri)

DEFINES += INSPECT_BASEDESIGN

INCLUDEPATH += \
#    $$REPORT_PATH/ \
#    $$REPORT_PATH/items \

SOURCES += \
    $$PWD/Dispositivos/nqcamaras.cpp \
    $$PWD/nqtglobal.cpp

contains(CONFIG, zint){
#    SOURCES += $$REPORT_PATH/items/lrbarcodeitem.cpp
}

HEADERS += \
    $$PWD/Dispositivos/nqcamaras.h \
    $$PWD/nqtglobal.h

contains(CONFIG,zint){
#    HEADERS += $$REPORT_PATH/items/lrbarcodeitem.h
}

FORMS += \
#    $$REPORT_PATH/databrowser/lrsqleditdialog.ui \

RESOURCES += \
#    $$REPORT_PATH/objectinspector/lobjectinspector.qrc \

