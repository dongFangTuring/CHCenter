QT       += core gui serialport 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation charts opengl

RC_FILE = CHCenter\exe_ico.rc
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CCONFIG -= x86_64
CONFIG += c++11 x86

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    CHCenter/aboutform.cpp \
    CHCenter/attitudeindicator.cpp \
    CHCenter/baseform.cpp \
    CHCenter/chartwindow.cpp \
    CHCenter/chcomform.cpp \
    CHCenter/chserialport.cpp \
    CHCenter/chsettingform.cpp \
    CHCenter/csvlogform.cpp \
    CHCenter/main.cpp \
    CHCenter/threedform.cpp \
    packet/imu_data_decode.cpp \
    packet/packet.cpp \
    mdbus/mdbus.cpp

HEADERS += \
    CHCenter/aboutform.h \
    CHCenter/attitudeindicator.h \
    CHCenter/baseform.h\
    CHCenter/chartwindow.h \
    CHCenter/chcomform.h \
    CHCenter/chserialport.h \
    CHCenter/chsettingform.h \
    CHCenter/csvlogform.h \
    CHCenter/threedform.h \
    packet/imu_data_decode.h \
    packet/packet.h \
    mdbus/mdbus.h

FORMS += \
    CHCenter/aboutform.ui \
    CHCenter/baseform.ui \
    CHCenter/chartwindow.ui \
    CHCenter/chcomform.ui \
    CHCenter/chsettingform.ui \
    CHCenter/csvlogform.ui \
    CHCenter/threedform.ui

TRANSLATIONS += \
    CHCenter/CHCenter_en_US.ts\
    CHCenter/CHCenter_tc.ts\
    CHCenter/CHCenter_cn.ts

LIBS += -lopengl32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    CHCenter/resources.qrc
