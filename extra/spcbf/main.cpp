#include <iostream>
#include <string>
#include <stdio.h>
#include "include/definitions.h"
#include "serial/qcdm_serial.h"
#include "util/hexdump.h"
#include "qc/dm.h"

using namespace std;
using namespace serial;


uint8_t enable_laf[] = { 0x3A, 0xA1, 0x6E, 0x7E };
uint8_t buffer[BUFFER_SIZE] = {};
void usage();
int main(int argc, char **argv);

void usage() {
	printf("\n\nSPC Brute Force:\n");
	printf("Usage:\n\n");
	printf("\tspcbf [PORT] [START] [END]\n");
	printf("\tspcbf\\\\.\\COM10 0 999999\n");
	printf("\tspcbf /dev/ttyUSB0 0 999999\n");
}


int main(int argc, char **argv) {
    string spc;    
	size_t bytesWritten,
	size_t bytesRead;
	QcdmSpcRequest req;
	QcdmSpcResponse resp;
	int start;
	int end;

	if (argc < 2) {
        usage();
        return 0;
    }

    start = strtoul(argv[2], NULL, 10);
    end   = strtoul(argv[3], NULL, 10);

    if (end < start) {
        usage();
        return 0;
    }

    Serial port("", 115200, serial::Timeout::simpleTimeout(500));

    try {
        port.setPort(argv[1]);
        port.open();
    } catch (serial::IOException e) {
        cout << e.what() << endl;
        return 1;
    }





    return 0;
}

