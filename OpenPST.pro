#-------------------------------------------------
#
# Project created by QtCreator 2015-05-31T13:35:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenPST
TEMPLATE = app

INCLUDEPATH += $$PWD/include $$PWD/lib/serial/include $$PWD/src

SOURCES +=\
    include/util.cpp \
    src/main.cpp \
    lib/serial/src/serial.cc \
    lib/serial/src/impl/unix.cc \
    lib/serial/src/impl/list_ports/list_ports_linux.cc \
    lib/serial/src/impl/list_ports/list_ports_osx.cc \
    lib/serial/src/impl/win.cc \
    src/gui/main_window.cpp \
    src/gui/mbn_tool_window.cpp \
    src/gui/sahara_window.cpp \
    src/gui/send_raw_window.cpp \
    src/helper/sahara_helper.cpp \
    src/serial/sahara_serial.cpp


linux-g++ {
    SOURCES +=  
}

win32 {
    SOURCES +=  
}

macx {
    SOURCES +=  
}

HEADERS  += include/definitions.h \
    include/mbn.h \
    include/qc_diag.h \
    include/qc_dload.h \
    include/qc_nv.h \
    include/qc_sahara.h \
    include/qc_streaming_dload.h \
    include/util.h \
    lib/serial/include/serial/impl/unix.h \
    lib/serial/include/serial/serial.h \
    lib/serial/include/serial/v8stdint.h \
    src/helper/sahara_helper.h \
    src/gui/main_window.h \
    src/gui/mbn_tool_window.h \
    src/gui/sahara_window.h \
    src/gui/send_raw_window.h \
    src/serial/sahara_serial.h

FORMS    += resources/ui/main_window.ui \
    resources/ui/mbn_tool_window.ui \
    resources/ui/sahara_window.ui \
    resources/ui/send_raw_window.ui
