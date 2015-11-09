#include <iostream>
#include <string>
#include <stdio.h>
#include "include/definitions.h"
#include "serial/serial.h"
#include "util/hexdump.h"
#include "laf.h"
#include "qc/hdlc.h"

#define DEBUG
#define HEXDUMP_PORT_TX
#define HEXDUMP_PORT_RX
#define BUFFER_SIZE (LAF_MAX_DATA_SIZE + sizeof(LafCmdHeader))

using namespace std;
using namespace serial;


uint8_t enable_laf[] = { 0x3A, 0xA1, 0x6E, 0x7E };
uint8_t buffer[BUFFER_SIZE] = {};
void usage();
int main(int argc, char **argv);

void usage() {
	printf("\n\nLG LAF Shell:\n");
	printf("Usage:\n\n");
	printf("\tlafshell [PORT]\n");
	printf("\tlafshell\\\\.\\COM10\n");
	printf("\tlafshell /dev/ttyUSB0\n");
}


int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
        return 0;
    }
    
    Serial port("", 115200, serial::Timeout::simpleTimeout(500));
    string command;
    
	size_t bytesWritten,
		bytesRead;

    try {
        port.setPort(argv[1]);
        port.open();
    }
    catch (serial::IOException e) {
        cout << e.what() << endl;
        return 1;
    }
    
	cout << "Special Commands: ENTER, LEAVE" << endl;

	int lastCommandSize = 0;

    do {
        cout << "# ";

        getline(cin, command);
        
        if (command.compare("ENTER") == 0) {
            bytesWritten = port.write(enable_laf, sizeof(enable_laf));
            bytesRead = port.read(buffer, BUFFER_SIZE);
            continue;
        } else if (command.compare("LEAVE") == 0) {

		} else if (command.compare("SPECIAL") == 0) {
			LafCmdHeader header = {};
			header.command = LAF_CMD_INFO;
			header.magic = 0x000;
			header.arg1 = 0xB08;
			header.arg_opt0 = LAF_CMD_INFO_SPRO;
			bytesWritten = port.write((uint8_t*)&header, sizeof(enable_laf));
			bytesRead = port.read(buffer, BUFFER_SIZE);
			hexdump(buffer, bytesRead);
			continue;
		}

        LafCmdHeader header = {}; 
        header.command = LAF_CMD_EXECUTE;
		header.magic = LAF_EXEC_MAGIC;

        LafCmd* packet = (LafCmd*)buffer;
        packet->header = header;

        if (command.size()) {
			lastCommandSize = 0;
            memcpy(packet->data, command.c_str(), command.size()); 
			memset(packet->data + command.size(), 0x00, 1);
			packet->header.size = command.size() + 1;
			packet->header.crc = 0x000000; 
			
			uint16_t crc = crc16((char*)packet, (sizeof(header) + command.size() + 1));
			packet->header.crc = crc; 
			bytesWritten = port.write((uint8_t*)packet, (sizeof(header) + command.size() + 1));
			//printf("Write %d\n", bytesWritten);
			//hexdump((uint8_t*)packet, bytesWritten);
		}

        while (true) {
            bytesRead = port.read(buffer, BUFFER_SIZE);

            if (bytesRead == 0) {
                break;
            }

			LafCmd* resp = (LafCmd*)buffer;

			if (resp->header.command == LAF_CMD_FAIL) {				
				cout << "Failed" << endl;
			} else {
				for (int i = 0; i < resp->header.size; i++) {
					cout << resp->data[i];
				}
				cout << endl;
			}
        }


        command.empty();
    } while (true);

    
    return 0;
}

