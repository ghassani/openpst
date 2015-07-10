#-------------------------------------------------
# PLACEHOLDER
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11

TARGET = qcdm

TEMPLATE = app

INCLUDEPATH += $$PWD/../lib/serial/include $$PWD/../src

DEPENDPATH += $$PWD/../

VPATH += $$PWD/../

SOURCES += \
    src/util/hexdump.cpp \
    src/util/endian.cpp \
    src/util/sleep.cpp \
    src/qc/hdlc.cpp \
    src/qc/crc.cpp \
    src/gui/qcdm_window.cpp \
    src/serial/hdlc_serial.cpp \
    src/serial/qcdm_serial.cpp \
    src/qcdm.cpp

HEADERS  += \
    src/include/definitions.h \
    src/qc/sahara.h \
    src/qc/crc.h \
    src/qc/hdlc.h \
    src/util/hexdump.h \
    src/util/endian.h \
    src/util/sleep.h \
    src/gui/qcdm_window.h \
    src/serial/qcdm_serial.h \
    src/serial/hdlc_serial.h


FORMS  += resources/ui/qcdm_window.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/ -lserial
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -lserial
else:unix: LIBS += -L$$OUT_PWD/ -lserial
