#-------------------------------------------------
# PLACEHOLDER
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11

TARGET = streaming_dload

TEMPLATE = app

INCLUDEPATH += $$PWD/../src $$PWD/../lib/serial/include

DEPENDPATH += $$PWD/../

VPATH += $$PWD/../

SOURCES += \
    src/util/hexdump.cpp \
    src/util/endian.cpp \
    src/util/sleep.cpp \
    src/qc/hdlc.cpp \
    src/qc/crc.cpp \
    src/gui/streaming_dload_window.cpp \
    src/serial/hdlc_serial.cpp \
    src/serial/streaming_dload_serial.cpp \
    src/worker/streaming_dload_read_worker.cpp \
    src/worker/streaming_dload_stream_write_worker.cpp \
    src/gui/application.cpp \
    src/streaming_dload.cpp

HEADERS  += \
    src/include/definitions.h \
    src/qc/streaming_dload.h \
    src/qc/crc.h \
    src/qc/hdlc.h \
    src/util/hexdump.h \
    src/util/endian.h \
    src/util/sleep.h \
    src/gui/streaming_dload_window.h \
    src/serial/streaming_dload_serial.h \
    src/worker/streaming_dload_read_worker.h \
    src/worker/streaming_dload_stream_write_worker.h \
    src/gui/application.h \
    src/serial/hdlc_serial.h


FORMS  += resources/ui/streaming_dload_window.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/  -llibopenpst
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -llibopenpst
else:unix: LIBS += -L$$OUT_PWD/ -llibopenpst
