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
    src/gui/application.cpp \
    src/gui/qcdm_window.cpp \
    src/qcdm.cpp

HEADERS  += \
    src/util/hexdump.h \
    src/gui/qcdm_window.h \
    src/gui/application.h 


FORMS  += resources/ui/qcdm_window.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/  -llibopenpst
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -llibopenpst
else:unix: LIBS += -L$$OUT_PWD/ -llibopenpst
