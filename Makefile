all: openpst

openpst:	
	qmake -makefile -o ./build/Makefile OpenPST.pro 
	$(MAKE) -C build

travis:
	/opt/qt54/bin/qt54-env.sh 
	/opt/qt54/bin/qmake -makefile -o ./build/Makefile OpenPST.pro 
	
clean:
	rm -rf build