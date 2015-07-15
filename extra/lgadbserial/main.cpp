#include <iostream>
#include <string>
#include <stdio.h>
#include "serial/serial.h"
#ifdef DEBUG
#include "util/hexdump.h"
#endif

#ifdef WIN32
#include "win_inttypes.h";
#include "win_inttypes.h";
#endif

using namespace std;
using namespace serial;

typedef struct {
	uint8_t command[20];
	uint32_t length;
	uint16_t crc;
	uint8_t unknown[5];
} lg_serial_cmd_header_t;

typedef struct {
	lg_serial_cmd_header_t header;
	uint8_t* data[0];
} lg_serial_cmd_t;

void usage();

int main(int argc, char **argv) {
  if (argc < 2) {
	  usage();
	  return 0;
  }
	
  Serial port("", 115200, serial::Timeout::simpleTimeout(1000));
  string cmdBuffer;
  string readBuffer;
  size_t bytesWritten,
	  bytesRead,
	  totalRead;

  vector<string> history;

  try {
	  port.setPort(argv[1]);
	  port.open();
  }
  catch (serial::IOException e) {
	  cout << e.what() << endl;
	  return 1;
  }
 
  lg_serial_cmd_header_t cmd = {};
  memcpy(cmd.command, "EXEC", 4);
  cmd.unknown[0] = 0x00;
  cmd.unknown[1] = 0x00;
  cmd.unknown[2] = 0xba;
  cmd.unknown[3] = 0xa7;
  cmd.unknown[4] = 0xba;
  cmd.unknown[5] = 0xbc;
  cmd.crc = 0x355d;

  do {
	  cout << "$ ";

	  getline(cin, cmdBuffer);
	  	  
	  if (cmdBuffer.compare("ENTER")) {

	  } else if (cmdBuffer.compare("LEAVE")) {

	  }

	  string historyEntry = cmdBuffer;

	  history.insert(history.end(), historyEntry);

	  cmd.length = cmdBuffer.size() + 1;
	  
	  if (cmdBuffer.length() < 4) {
		  while (cmdBuffer.length() < 4) {
			  cmdBuffer.insert(cmdBuffer.end(), 1, 0x00);
		  }
	  }

	  cmdBuffer.insert(0, reinterpret_cast<const char*>(&cmd), sizeof(cmd));

	  bytesWritten = port.write((uint8_t*)&cmdBuffer[0], cmdBuffer.size());

#ifdef DEBUG
	  hexdump_tx((uint8_t*)&cmdBuffer[0], bytesWritten);
#endif

	  totalRead = 0;
	  
	  do{
		  bytesRead = port.read(readBuffer, 1024);

		  if (bytesRead == 0) {
			  break;
		  }

		  totalRead += bytesRead;
		  
	  } while (true);

#ifdef DEBUG	  
	  hexdump((uint8_t*)&readBuffer[0], readBuffer.size());
#endif
	  if (totalRead) {
		  cout << readBuffer;
	  } else {
		  cout << "" << endl;
	  }
	  

	  cmdBuffer.empty();
	  readBuffer.empty();
	  cmdBuffer.shrink_to_fit();
	  readBuffer.shrink_to_fit();
  } while (true);

  cout << "Dumping History:\n" << endl;
  for (auto h : history) {
	  cout << h << endl;
  }

  return 0;
}



void usage() {
	printf("\n\nLG ADB over Fastboot Serial:\n");
	printf("Usage:\n\n");
	printf("\tlgadbserial [PORT]\n");
	printf("\tlgadbserial \\\\.\\COM10\n");
	printf("\tlgadbserial /dev/ttyUSB0\n");
}
