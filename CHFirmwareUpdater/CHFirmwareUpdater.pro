QT       += core gui serialport
RC_FILE = exe_ico.rc

# 支持QCustomPlot
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11
QMAKE_CXXFLAGS += /utf-8
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    kptl/kboot_protocol.cpp \
    kptl/kptl.c \
    main.cpp \
    mainwindow.cpp \
    mdbus_dialog.cpp \
    utilities/hex2bin.cpp \
    utilities/mdbus.cpp \
    utilities/serial.cpp \
    utilities/test2.cpp

HEADERS += \
    kptl/kboot_protocol.h \
    kptl/kptl.h \
    mainwindow.h \
    mdbus_dialog.h \
    utilities/hex2bin.h \
    utilities/mdbus.h \
    utilities/serial.h \
    utilities/test2.h

FORMS += \
    mainwindow.ui \
    mdbus_dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
