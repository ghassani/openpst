#-------------------------------------------------
# PLACEHOLDER
#-------------------------------------------------

CONFIG += C++11 staticlib

TARGET = libopenpst

TEMPLATE = lib

INCLUDEPATH += $$PWD/../src $$PWD/../lib/serial/include

DEPENDPATH += $$PWD/../

VPATH += $$PWD/../

HEADERS  += \
    src/include/definitions.h \
    src/include/win_inttypes.h \
    src/include/win_stdint.h \
    src/qc/crc.h \
    src/qc/diag.h \
    src/qc/diag_nv.h \
    src/qc/dload.h \
    src/qc/hdlc.h \
    src/qc/mbn.h \
    src/qc/qcdm_nv_responses.h \
    src/qc/qcdm_packet_types.h \
    src/qc/sahara.h \
    src/qc/streaming_dload.h \
    src/serial/hdlc_serial.h \
    src/serial/qcdm_serial.h \
    src/serial/sahara_serial.h \
    src/serial/streaming_dload_serial.h \
    src/util/convert.h \
    src/util/endian.h \
    src/util/hexdump.h \
    src/util/sleep.h 

SOURCES += \
    src/qc/crc.cpp \
    src/qc/hdlc.cpp \
    src/serial/hdlc_serial.cpp \
    src/serial/qcdm_serial.cpp \
    src/serial/sahara_serial.cpp \
    src/serial/streaming_dload_serial.cpp \
    src/util/convert.cpp \
    src/util/endian.cpp \
    src/util/hexdump.cpp \
    src/util/sleep.cpp 

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/ -lserial
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -lserial
else:unix: LIBS += -L$$OUT_PWD/ -lserial