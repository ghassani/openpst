all: openpst

openpst:	
	qmake -makefile -o ./build/Makefile OpenPST.pro 
	$(MAKE) -C build

clean:
	rm -rf build
