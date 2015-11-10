all: openpst

openpst:	
	qmake -makefile -o ./build/Makefile OpenPST.pro 
	$(MAKE) -C build

travis:
	/opt/qt54/bin/qt54-env.sh 
	/opt/qt54/bin/qmake -makefile -o ./build/Makefile OpenPST.pro 

libopenpst:
	$(CXX) \
		-I"./lib/serial/include" \
		-I"./src" \
	    src/qc/dm_efs_manager.cpp \
	    src/qc/dm_efs_node.cpp \
	    src/qc/hdlc.cpp \
	    src/serial/hdlc_serial.cpp \
	    src/serial/qcdm_serial.cpp \
	    src/serial/sahara_serial.cpp \
	    src/serial/streaming_dload_serial.cpp \
	    src/util/convert.cpp \
	    src/util/endian.cpp \
	    src/util/hexdump.cpp \
	    src/util/sleep.cpp 
		-o ./build/libopenpst \
		-O0 -g3 -std=c++11 -Wall

clean:
	rm -rf build