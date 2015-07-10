#-------------------------------------------------
# PLACEHOLDER
#-------------------------------------------------

TEMPLATE = subdirs
SUBDIRS = serial sahara streaming_dload qcdm mbn_tool

serial.file = qmake/serial.pro
sahara.file = qmake/sahara.pro
streaming_dload.file = qmake/streaming_dload.pro
qcdm.file = qmake/qcdm.pro
mbn_tool.file = qmake/mbn_tool.pro

sahara.depends = serial
streaming_dload.depends = serial
qcdm.depends = serial
