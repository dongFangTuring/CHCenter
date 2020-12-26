QT       += core gui serialport 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CCONFIG -= x86_64
CONFIG += c++11 x86

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    attitudeindicator.cpp \
    baseform.cpp \
    chartwindow.cpp \
    chcomform.cpp \
    chserialport.cpp \
    chsettingform.cpp \
    csvlogform.cpp \
    main.cpp \
    packet/imu_data_decode.cpp \
    packet/packet.cpp \
    packet/packet.cpp \
    threedform.cpp

HEADERS += \
    attitudeindicator.h \
    baseform.h\
    chartwindow.h \
    chcomform.h \
    chserialport.h \
    chsettingform.h \
    csvlogform.h \
    packet/imu_data_decode.h \
    packet/packet.h \
    threedform.h

FORMS += \
    baseform.ui \
    chartwindow.ui \
    chcomform.ui \
    chsettingform.ui \
    csvlogform.ui \
    threedform.ui

TRANSLATIONS += \
    CHCenter_en_US.ts
    CHCenter_tc.ts
    CHCenter_cn.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
