#-------------------------------------------------
# PLACEHOLDER
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11

TARGET = sahara

TEMPLATE = app

INCLUDEPATH += $$PWD/lib/serial/include $$PWD/src

SOURCES += \
    lib/serial/src/serial.cc \
    src/util/hexdump.cpp \
    src/util/endian.cpp \
    src/util/sleep.cpp \
    src/gui/sahara_window.cpp \
    src/serial/sahara_serial.cpp \
    src/worker/sahara_memory_read_worker.cpp \
    src/worker/sahara_image_transfer_worker.cpp \
    src/sahara.cpp

HEADERS  += \
    src/include/definitions.h \
    src/qc/sahara.h \
    src/util/hexdump.h \
    src/util/endian.h \
    src/util/sleep.h \
    src/gui/sahara_window.h \
    src/serial/sahara_serial.h \
    src/worker/sahara_memory_read_worker.h \
    src/worker/sahara_image_transfer_worker.h


FORMS  += resources/ui/sahara_window.ui 

