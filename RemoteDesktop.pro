QT       += core gui opengl concurrent network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
include($$PWD/AudioModule/audio.pri)
include($$PWD/DesktopModule/desktop.pri)
include($$PWD/LogModule/QsLog.pri)
include($$PWD/MouseModule/mouse.pri)
include($$PWD/VideoModule/video.pri)
include($$PWD/SignalServerModule/signalserver.pri)
include($$PWD/KeywordModule/keyword.pri)
include($$PWD/DisplayModule/display.pri)
include($$PWD/Example/example.pri)
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_NO_KEYWORDS
DEFINES += WEBRTC_WIN
DEFINES += NOMINMAX
DEFINES += WIN32_LEAN_AND_MEAN
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/webrtc
DEPENDPATH += $$PWD/include/webrtc
INCLUDEPATH += $$PWD/include/webrtc/third_party/abseil-cpp
DEPENDPATH += $$PWD/include/webrtc/third_party/abseil-cpp

SOURCES += \
    data_control.cpp \
    databuffer_task_thread.cpp \
    datachannel_bundle_prase.cpp \
    interface.cpp \
    peerconnection_manager.cpp \
    remote_desktop.cpp

HEADERS += \
    RemoteDesktop_global.h \
    data_control.h \
    databuffer_task_thread.h \
    datachannel_bundle_prase.h \
    interface.h \
    message_protocol_define.h \
    peerconnection_manager.h \
    remote_desktop.h

# Default rules for deployment.

unix {
    DEFINES += WEBRTC_LINUX
    DEFINES += WEBRTC_POXY
    CONFIG(debug, debug|release){
        DESTDIR =$$PWD/bin/linux/debug
        PRE_TARGETDEPS += $$PWD/lib/linux/debug/libwebrtcd.a
        LIBS += -L$$PWD/lib/linux/debug/ -lwebrtc
        LIBS += -L$$PWD/lib/linux/debug/ -llibyuv
    }else{
        DESTDIR =$$PWD/bin/linux/release
        PRE_TARGETDEPS += $$PWD/lib/linux/release/libwebrtc.a
        LIBS += -L$$PWD/lib/linux/release/ -lwebrtc
        LIBS += -L$$PWD/lib/linux/release/ -llibyuv
    }

}

win32{
    DEFINES += WEBRTC_WIN
    DEFINES += NOMINMAX
    DEFINES += WIN32_LEAN_AND_MEAN
    CONFIG(debug, debug|release){

        PRE_TARGETDEPS += $$PWD/lib/win32/debug/webrtc.lib
        LIBS += -L$$PWD/lib/win32/debug/ -lwebrtc
        LIBS += -L$$PWD/lib/win32/debug/ -llibyuv
    }else{
        DESTDIR =$$PWD/bin/win32/release
        PRE_TARGETDEPS += $$PWD/lib/win32/release/webrtc.lib
        LIBS += -L$$PWD/lib/win32/release/ -lwebrtc
        LIBS += -L$$PWD/lib/win32/release/ -llibyuv
    }
    LIBS += -lwinmm -lAdvapi32 -lGdi32 -lD3D11 -lDXGI -lws2_32 -lStrmiids
    LIBS += -lOle32 -loleaut32 -luser32 -lIphlpapi -ldxgi -lcrypt32
    LIBS += -lsecur32 -ldmoguids -lwmcodecdspuuid -lamstrmid -lmsdmo -lkernel32 -lwinspool
    LIBS += -lshell32 -luuid -lcomdlg32
    LIBS += -luser32
    QMAKE_CFLAGS_DEBUG += -MTd
    QMAKE_CXXFLAGS_DEBUG += -MTd
    QMAKE_CFLAGS_RELEASE += -MT
    QMAKE_CXXFLAGS_RELEASE += -MT
}

!isEmpty(target.path): INSTALLS += target



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target





