all: install_deps openpst

install_deps:
 	@git clone https://github.com/wjwwood/serial.git lib/serial --depth=1
	@git clone https://github.com/apache/xerces-c.git lib/xerces --depth=1
	@git clone https://github.com/ghassani/meid-converter-cpp lib/meidconverter --depth=1

openpst:	
	qmake -makefile -o ./build/Makefile OpenPST.pro 
	$(MAKE) -C build

clean:
	rm -rf build