DEFINES += INSPECT_BASEDESIGN

INCLUDEPATH += \
#    $$REPORT_PATH/ \
#    $$REPORT_PATH/items \

SOURCES += \
    $$PWD/nqtglobal.cpp \
    $$PWD/nQTrucksEngine.cpp


contains(CONFIG, zint){
#    SOURCES += $$REPORT_PATH/items/lrbarcodeitem.cpp
}

HEADERS += \
    $$PWD/nqtglobal.h \
    $$PWD/nQTrucksEngine_p.h \
    $$PWD/nQTrucksEngine.h


contains(CONFIG,zint){
#    HEADERS += $$REPORT_PATH/items/lrbarcodeitem.h
}

FORMS += \
#    $$REPORT_PATH/databrowser/lrsqleditdialog.ui \

RESOURCES += \
#    $$REPORT_PATH/objectinspector/lobjectinspector.qrc \


#** CAMARAS  **#
SOURCES += \
    $$PWD/Dispositivos/CamaraIP.cpp

HEADERS += \
    $$PWD/Dispositivos/CamaraIP.h

OTHER_FILES += \
        $$PWD/Dispositivos/cameras.yaml

#** NEWSAGES I/O **#
SOURCES += \
    $$PWD/Dispositivos/NewsagesIO.cpp
HEADERS += \
    $$PWD/Dispositivos/NewsagesIO.h

OTHER_FILES += \
        $$PWD/Dispositivos/StandardFirmata/StandardFirmata.ino \
        $$PWD/Dispositivos/Firmata/promini.png

#** BASCULAS **#
SOURCES += \
    $$PWD/Dispositivos/Basculas/nQSerialPortReader.cpp

HEADERS += \
    $$PWD/Dispositivos/Basculas/nQSerialPortReader.h

OTHER_FILES += \
    $$PWD/Dispositivos/Basculas/bascula_puerto.hex \
    $$PWD/Dispositivos/Basculas/emulador_bascula_puerto/emulador_bascula_puerto.ino



#** AUXILIARES  **#
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


#** ALPR  **#
SOURCES += \
    $$PWD/Dispositivos/NewsagesAlpr.cpp

HEADERS += \
    $$PWD/Dispositivos/NewsagesAlpr.h

