include(../common.pri)

DEFINES += INSPECT_BASEDESIGN

INCLUDEPATH += \
#    $$REPORT_PATH/ \
#    $$REPORT_PATH/items \

SOURCES += \
    $$PWD/nqtglobal.cpp \
    $$PWD/nQTrucksEngine.cpp \
    $$PWD/Dispositivos/CamaraIP.cpp \
    $$PWD/Dispositivos/NewsagesIO.cpp \
    $$PWD/Dispositivos/nQSerialPort.cpp

contains(CONFIG, zint){
#    SOURCES += $$REPORT_PATH/items/lrbarcodeitem.cpp
}

HEADERS += \
    $$PWD/nqtglobal.h \
    $$PWD/nQTrucksEngine_p.h \
    $$PWD/nQTrucksEngine.h \
    $$PWD/Dispositivos/CamaraIP.h \
    $$PWD/Dispositivos/NewsagesIO.h \
    $$PWD/Dispositivos/nQSerialPort.h

contains(CONFIG,zint){
#    HEADERS += $$REPORT_PATH/items/lrbarcodeitem.h
}

FORMS += \
#    $$REPORT_PATH/databrowser/lrsqleditdialog.ui \

RESOURCES += \
#    $$REPORT_PATH/objectinspector/lobjectinspector.qrc \

