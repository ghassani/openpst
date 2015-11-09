#include <iostream>
#include <string>
#include <stdio.h>
#include "include/definitions.h"
#include "serial/qcdm_serial.h"
#include "util/hexdump.h"
#include "qc/dm.h"
#include "qc/dm_efs_manager.h"

using namespace std;
using namespace serial;
using namespace OpenPST;

QcdmSerial port("", 115200, serial::Timeout::simpleTimeout(500));
DmEfsManager efsManager(port);

void usage();
bool processItem(int item);
int main(int argc, char **argv);

void usage() {
	printf("\n\nSamsung SIM Unlock for Qualcomm SOC:\n");
	printf("Usage:\n\n");
	printf("\tqcsamunlock [PORT]\n");
	printf("\tqcsamunlock\\\\.\\COM10\n");
	printf("\tqcsamunlock /dev/ttyUSB0\n");
}


int main(int argc, char **argv) {
	
	if (argc < 2) {
        usage();
        return 0;
    }

    try {
        port.setPort(argv[1]);
        port.open();
    } catch (serial::IOException e) {
        cout << e.what() << endl;
        return 1;
    }

	QcdmEfsStatfsResponse statResponse;

	if (efsManager.statfs("/", statResponse) == efsManager.kDmEfsSuccess) {
		
		if (!processItem(10080) || !processItem(10074) || !processItem(10073)) {
			cout << "Operation Failed!" << endl;
			return 1;
		}

	} else {
		cout << "Error checking for EFS access" << endl;
		return 1;
	}


	QcdmEfsSyncResponse syncResponse;
	QcdmEfsGetSyncStatusResponse syncStatusResponse;
	try {
		syncResponse = efsManager.syncNoWait("/");
	}
	catch (std::exception e) {
		cout << "Exception: " << e.what() << endl;
		return 1;
	}
	
	syncStatusResponse = efsManager.getSyncStatus(syncResponse.token);

	cout << "SYNC STATUS: " << syncStatusResponse.status << endl;

    return 0;
}

bool processItem(int item)
{
	ostringstream itemStrStream; 
	std::string path = "/public/../nvm/num/";
	itemStrStream << item;
	path.append(itemStrStream.str());

	if (efsManager.mkdir("public", 0x00) == efsManager.kDmEfsSuccess) {
		if (efsManager.deltree(path) == efsManager.kDmEfsSuccess) {
			return true;
		} else {
			cout << "Error removing nv item " << itemStrStream.str() << endl;
			return false;
		}
	}

	return false;
}