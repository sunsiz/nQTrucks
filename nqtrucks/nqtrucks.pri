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

#** FIRMATA   **#
HEADERS += \
        #$$PWD/Dispositivos/Firmata/qtfirmata_global.h \
        #$$PWD/Dispositivos/Firmata/plugin.h \
        $$PWD/Dispositivos/Firmata/utils.h \
        $$PWD/Dispositivos/Firmata/firmata.h \
        $$PWD/Dispositivos/Firmata/backends/backend.h \
        $$PWD/Dispositivos/Firmata/backends/serialport.h \
        $$PWD/Dispositivos/Firmata/backends/serialinfo.h \
        $$PWD/Dispositivos/Firmata/pins/pin.h \
        $$PWD/Dispositivos/Firmata/pins/digitalpin.h \
        $$PWD/Dispositivos/Firmata/pins/pwmpin.h \
        $$PWD/Dispositivos/Firmata/pins/analogpin.h \
        $$PWD/Dispositivos/Firmata/pins/servo.h \
        $$PWD/Dispositivos/Firmata/pins/encoder.h \
        $$PWD/Dispositivos/Firmata/pins/i2c.h

SOURCES += \
        #$$PWD/Dispositivos/Firmata/plugin.cpp \
        $$PWD/Dispositivos/Firmata/firmata.cpp \
        $$PWD/Dispositivos/Firmata/backends/backend.cpp \
        $$PWD/Dispositivos/Firmata/backends/serialport.cpp \
        $$PWD/Dispositivos/Firmata/backends/serialinfo.cpp \
        $$PWD/Dispositivos/Firmata/pins/pin.cpp \
        $$PWD/Dispositivos/Firmata/pins/digitalpin.cpp \
        $$PWD/Dispositivos/Firmata/pins/pwmpin.cpp \
        $$PWD/Dispositivos/Firmata/pins/analogpin.cpp \
        $$PWD/Dispositivos/Firmata/pins/servo.cpp \
        $$PWD/Dispositivos/Firmata/pins/encoder.cpp \
        $$PWD/Dispositivos/Firmata/pins/i2c.cpp

OTHER_FILES += \
        $$PWD/Dispositivos/StandardFirmata/StandardFirmata.ino \
        $$PWD/Dispositivos/Firmata/promini.png

