#include <iostream>
#include <string>
#include <stdio.h>
#include "serial/serial.h"

using namespace std;
using namespace serial;

int run(int argc, char **argv);
void hexdump(unsigned char *data, unsigned int amount);
uint16_t crc16(const char* buffer, size_t len);
void usage();

/* Table of CRCs for each possible byte, with a generator polynomial of 0x8408 */
static const uint16_t crc_table[256] = {
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
    0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
    0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
    0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
    0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
    0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
    0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
    0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
    0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
    0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
    0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

/*
struct exec_cmd {
	cmd[4];
	
};*/

int main(int argc, char **argv) {
  if (argc < 2) {
	  usage();
	  return 0;
  }
	
  try {
    return run(argc, argv);
  } catch (std::exception &e) {
	  std::cerr << "Error: " << e.what() << std::endl;
  }
}

typedef struct {
	uint8_t command[20];
	uint32_t length;
	uint16_t crc;
	uint8_t unknown[5];
} lg_serial_cmd_header_t;

typedef struct {
	lg_serial_cmd_header_t header;
	uint8_t data;
} lg_serial_cmd_t;

int run(int argc, char **argv) {
	Serial port(argv[1], 115200, serial::Timeout::simpleTimeout(1000));
	string cmdBuffer;
	string readBuffer;

	size_t bytesWritten, bytesRead;
	
	
	uint8_t header[] = {
		0x45, 0x58, 0x45, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x5d, 0x35,
		0x00, 0x00, 0xba, 0xa7, 0xba, 0xbc, 0x6c, 0x73, 0x00, 0x00
	};
	
	std::vector<serial::PortInfo> devices = serial::list_ports();
	std::vector<serial::PortInfo>::iterator iter = devices.begin();

	std::string comPort;

	//hexdump(header, sizeof(header));

	  
	uint8_t* writeBuffer = new uint8_t[1024];

	do {
		  cout << "$ ";
		  getline (cin, cmdBuffer);
		  
		  
		  lg_serial_cmd_header_t hdr;
		  memset(&hdr, 0, sizeof(hdr));
		  memcpy(hdr.command, "EXEC", 4);
		  hdr.length = cmdBuffer.size()+1;
		  hdr.unknown[0] = 0x00;
		  hdr.unknown[1] = 0x00;
		  hdr.unknown[2] = 0xba;
		  hdr.unknown[3] = 0xa7;
		  hdr.unknown[4] = 0xba;
		  hdr.unknown[5] = 0xbc;
		  
		  hdr.crc = 0x355d;
		  //hdr.crc = crc16((const char*)cmdBuffer.c_str(), cmdBuffer.size());
		 // printf("%02x\n", hdr.crc);
/*  hdr.crc[0] = crc & 0xFF;
		  hdr.crc[1] = (crc >> 8) & 0xFF;*/
		  
		  //packet.data = (uint8_t*) cmdBuffer.c_str();
		  //cmdBuffer.insert(0, sizeof(lg_serial_cmd_header_t), (uint8_t*)&hdr);
		  /*
		  memcpy(writeBuffer, &hdr, sizeof(hdr));
		  memcpy(&writeBuffer[sizeof(hdr)], cmdBuffer.c_str(), cmdBuffer.size());
		  
		  
		  memset(&writeBuffer[sizeof(hdr)+cmdBuffer.size()], 0x00, 2);

		  
		  //hexdump(writeBuffer,  sizeof(hdr)+cmdBuffer.size()+2);
		  
		  bytesWritten = port.write(writeBuffer, sizeof(hdr)+cmdBuffer.size()+2);

		  printf("Wrote %zd bytes\n", bytesWritten);
		  hexdump(writeBuffer, bytesWritten);
		  
		  
		  bytesRead = port.read(readBuffer);

		  
		  if (!bytesRead) {
			  printf("Device Did Not Response\n");			  
			  break;			  
		  }
		  printf("Read %zd bytes\n", bytesRead);

		  hexdump((uint8_t*)readBuffer.c_str(), bytesRead);
		  //cout << readBuffer.size() << endl;
		  //cout << readBuffer;
		  
		  */
		  
		  bytesWritten = port.write(header, sizeof(header));
		  bytesRead = port.read(readBuffer);
		  //hexdump(writeBuffer, bytesRead);
		  cout << readBuffer;
	  
	} while(true);

	delete writeBuffer;
	
	return 0;
}

void usage() {
	printf("\n\nLG ADB over Fastboot Serial:\n");
	printf("Usage:\n\n");
	printf("\tlgadbserial [PORT]\n");
	printf("\tlgadbserial \\\\.\\COM10\n");
	printf("\tlgadbserial /dev/ttyUSB0\n");
}

uint16_t crc16(const char* buffer, size_t len) {
    uint16_t crc = 0xffff;

    while (len--)
        crc = crc_table[(crc ^ *buffer++) & 0xff] ^ (crc >> 8);
    return ~crc;
}


void hexdump(unsigned char *data, unsigned int amount){
	unsigned int    dp, p;  /* data pointer */
	const char      trans[] =
		"................................ !\"#$%&'()*+,-./0123456789"
		":;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklm"
		"nopqrstuvwxyz{|}~...................................."
		"....................................................."
		"........................................";


	for (dp = 1; dp <= amount; dp++) {
		fprintf(stderr, "%02x ", data[dp - 1]);
		if ((dp % 8) == 0)
			fprintf(stderr, " ");
		if ((dp % 16) == 0) {
			fprintf(stderr, "| ");
			p = dp;
			for (dp -= 16; dp < p; dp++)
				fprintf(stderr, "%c", trans[data[dp]]);
			fflush(stderr);
			fprintf(stderr, "\n");
		}
		fflush(stderr);
	}
	// tail
	if ((amount % 16) != 0) {
		p = dp = 16 - (amount % 16);
		for (dp = p; dp > 0; dp--) {
			fprintf(stderr, "   ");
			if (((dp % 8) == 0) && (p != 8))
				fprintf(stderr, " ");
			fflush(stderr);
		}
		fprintf(stderr, " | ");
		for (dp = (amount - (16 - p)); dp < amount; dp++)
			fprintf(stderr, "%c", trans[data[dp]]);
		fflush(stderr);
	}
	fprintf(stderr, "\n");

	return;
}

/*
uint8_t readBuff[1024];
uint8_t test[] = {
	0x45, 0x58, 0x45, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x5d, 0x35,
	0x00, 0x00, 0xba, 0xa7, 0xba, 0xbc, 0x6c, 0x73, 0x00, 0x00
};

size_t bytesRead, bytesWritten;

bytesWritten = port.write(test, sizeof(test));

hexdump(test, bytesWritten);

sleep(2000);
bytesRead = port.read(readBuff, 1024);
hexdump(readBuff, bytesRead);

return 0;*/