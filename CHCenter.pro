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
    CHCenter/chserialport.cpp \
    CHCenter/chsettingform.cpp \
    CHCenter/csvlogform.cpp \
    CHCenter/fftchartwindow.cpp \
    CHCenter/form_com_select.cpp \
    CHCenter/main.cpp \
    CHCenter/threedform.cpp \
    kptl/kboot_protocol.cpp \
    mdbus/mdbus.cpp \
    utilities/fft.cpp \
    utilities/imu_parser.cpp

HEADERS += \
    CHCenter/aboutform.h \
    CHCenter/attitudeindicator.h \
    CHCenter/baseform.h\
    CHCenter/chartwindow.h \
    CHCenter/chserialport.h \
    CHCenter/chsettingform.h \
    CHCenter/csvlogform.h \
    CHCenter/fftchartwindow.h \
    CHCenter/form_com_select.h \
    CHCenter/threedform.h \
    kptl/kboot_protocol.h \
    mdbus/mdbus.h \
    utilities/fft.h \
    utilities/imu_parser.h

FORMS += \
    CHCenter/aboutform.ui \
    CHCenter/baseform.ui \
    CHCenter/chartwindow.ui \
    CHCenter/chsettingform.ui \
    CHCenter/csvlogform.ui \
    CHCenter/fftchartwindow.ui \
    CHCenter/form_com_select.ui \
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
