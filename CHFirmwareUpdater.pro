QT       += core gui serialport
RC_FILE = CHFirmwareUpdater/updater_ico.rc

# 支持QCustomPlot
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

QMAKE_CXXFLAGS += /utf-8
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CHFirmwareUpdater/form_display.cpp \
    kptl/kboot_protocol.cpp \
    mdbus/mdbus.cpp \
    utilities/imu_parser.cpp \
    utilities/hex2bin.cpp \
    utilities/serial.cpp \
    CHFirmwareUpdater/mdbus_dialog.cpp \
    CHFirmwareUpdater/updater_main.cpp \
    CHFirmwareUpdater/updater_mainform.cpp \


HEADERS += \
    CHFirmwareUpdater/form_display.h \
    kptl/kboot_protocol.h \
    mdbus/mdbus.h \
    utilities/hex2bin.h \
    utilities/imu_parser.h \
    utilities/serial.h \
    CHFirmwareUpdater/updater_mainform.h \
    CHFirmwareUpdater/mdbus_dialog.h \


FORMS += \
    CHFirmwareUpdater/form_display.ui \
    CHFirmwareUpdater/mdbus_dialog.ui \
    CHFirmwareUpdater/updater_mainform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
