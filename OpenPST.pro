#-------------------------------------------------
#
# Project created by QtCreator 2015-05-31T13:35:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11

TARGET = OpenPST
TEMPLATE = app

INCLUDEPATH += $$PWD/lib/serial/include $$PWD/src

SOURCES += \
    lib/serial/src/serial.cc \
    src/util/hexdump.cpp \
    src/util/endian.cpp \
    src/util/sleep.cpp \
    src/qc/hdlc.cpp \
    src/qc/crc.cpp \
    src/main.cpp \
    src/gui/main_window.cpp \
    src/gui/mbn_tool_window.cpp \
    src/gui/sahara_window.cpp \
    src/gui/send_raw_window.cpp \
    src/serial/sahara_serial.cpp \
    src/serial/dm_serial.cpp

HEADERS  += \
    lib/serial/include/serial/serial.h \
    lib/serial/include/serial/v8stdint.h \
    src/definitions.h \
    src/qc/mbn.h \
    src/qc/diag.h \
    src/qc/diag_nv.h \
    src/qc/dload.h \
    src/qc/sahara.h \
    src/qc/streaming_dload.h \
    src/qc/crc.h \
    src/qc/hdlc.h \
    src/util/hexdump.h \
    src/util/endian.h \
    src/util/sleep.h \
    src/gui/main_window.h \
    src/gui/mbn_tool_window.h \
    src/gui/sahara_window.h \
    src/gui/send_raw_window.h \
    src/serial/sahara_serial.h \
    src/serial/dm_serial.h

linux-g++ {
    SOURCES += \
    lib/serial/src/impl/unix.cc \
    lib/serial/src/impl/list_ports/list_ports_linux.cc

    HEADERS += \
    lib/serial/include/serial/impl/unix.h
}

win32 {
    SOURCES += \
    lib/serial/src/impl/win.cc \
    lib/serial/src/impl/list_ports/list_ports_win.cc

    HEADERS += \
    lib/serial/include/serial/impl/win.h
}

macx {
    SOURCES += \
    lib/serial/src/impl/unix.cc \
    lib/serial/src/impl/list_ports/list_ports_osx.cc

    HEADERS += \
    lib/serial/include/serial/impl/unix.h
}

FORMS    += resources/ui/main_window.ui \
    resources/ui/mbn_tool_window.ui \
    resources/ui/sahara_window.ui \
    resources/ui/send_raw_window.ui
