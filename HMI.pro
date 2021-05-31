QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += pc_stub
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

pc_stub{
# These files only are available on PC STUB environment
DEFINES += HMI_API_STUB

SOURCES +=

HEADERS +=

}else{



#LIBS += -L../../project/local/usr/lib/ \
#                -lpanda_dbus 

INCLUDEPATH +=  ../../project/host/sysroots/armv7ahf-vfp-neon-poky-linux-gnueabi/usr/lib/qt5/mkspecs/devices/linux-imx6-g++
SOURCES += FRAMEWORK/src/hmi_log.cpp

}

INCLUDEPATH +=  ../../project/host/sysroots/armv7ahf-vfp-neon-poky-linux-gnueabi/usr/include \
                $$PWD \
                $$PWD/API/inc \
                $$PWD/FRAMEWORK/inc \
                $$PWD/PAGE/base \
                $$PWD/PAGE/ctrl/inc \
                $$PWD/PAGE/lib/inc \
                $$PWD/PAGE/mode/inc \
                $$PWD/PAGE/view/inc

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    API/src/hmiapi_abstract.cpp \
    API/src/hmiapi_modemanager.cpp \    
    API/src/hmiapi_audiomanager.cpp   \
    FRAMEWORK/src/hmifrm_common.cpp \
    FRAMEWORK/src/hmifrm_modeclass.cpp \
    FRAMEWORK/src/hmifrm_viewclass.cpp \
    FRAMEWORK/src/hmifrm_apiclass.cpp \
    FRAMEWORK/src/hmifrm_abstractevent.cpp \
    FRAMEWORK/src/hmifrm_funcevent.cpp \
    FRAMEWORK/src/hmifrm_keyevent.cpp \
    FRAMEWORK/src/hmifrm_keyclass.cpp \
    FRAMEWORK/src/hmifrm_modeevent.cpp \
    FRAMEWORK/src/hmifrm_navitohmievent.cpp \
    FRAMEWORK/src/hmifrm_transevent.cpp \
    FRAMEWORK/src/hmifrm_modemanager.cpp \
    FRAMEWORK/src/hmifrm_abstractimportview.cpp\
    PAGE/mode/src/hmimode_common.cpp \
    PAGE/lib/src/hmilib_statusbar.cpp \
    PAGE/base/cursorctrlclass.cpp \
    PAGE/mode/src/hmimode_home.cpp\
    PAGE/view/src/hmiview_home.cpp

HEADERS += \
    mainwindow.h \
    API/inc/singleton.h \
    API/inc/hmiapi_abstract.h \
    API/inc/hmiapi_modemanager.h \
    API/inc/hmiapi_audiomanager.h \
    FRAMEWORK/inc/hmifrm_common.h \
    FRAMEWORK/inc/hmifrm_apiclass.h \
    FRAMEWORK/inc/hmifrm_viewclass.h \
    FRAMEWORK/inc/hmifrm_modeclass.h \
    FRAMEWORK/inc/hmifrm_abstractevent.h \
    FRAMEWORK/inc/hmifrm_funcevent.h \
    FRAMEWORK/inc/hmifrm_keyevent.h \
    FRAMEWORK/inc/hmifrm_modevent.h \
    FRAMEWORK/inc/hmifrm_navitohmievent.h \
    FRAMEWORK/inc/hmifrm_transevent.h \
    FRAMEWORK/inc/hmifrm_modemanager.h \
    FRAMEWORK/inc/hmifrm_abstractimportview.h\
    FRAMEWORK/inc/hmifrm_keyclass.h \
    FRAMEWORK/inc/hmi_log.h \
    PAGE/lib/inc/hmilib_statusbar.h \
    PAGE/base/cursorctrlclass.h \
    PAGE/mode/inc/hmimode_common.h \
    PAGE/view/inc/hmiview_home.h \
    PAGE/mode/inc/hmimode_home.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
