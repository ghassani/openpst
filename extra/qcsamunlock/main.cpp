/**
* @file main.cpp
* @package qcsamunlock
* @brief QCDM response conversion helper functions
*
*  Copyright (C) Gassan Idriss <ghassani@gmail.com>
*
* This program is free software ; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation ; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY ; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with the program ; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <iostream>
#include <string>
#include <stdio.h>
#include "include/definitions.h"
#include "util/sleep.h"
#include "serial/qcdm_serial.h"
#include "qc/dm.h"
#include "qc/dm_efs_manager.h"

using namespace std;
using namespace serial;
using namespace OpenPST;

QcdmSerial port("", 115200, serial::Timeout::simpleTimeout(500));
DmEfsManager efsManager(port);

/**
* @brief usage - Show program usage information
* @return void
*/
void usage() {
	printf("---------------------------------------------------\n");
	printf(" Samsung SIM Unlock for Qualcomm SOC:              \n");
	printf("---------------------------------------------------\n");
	printf(" Usage:\n");
	printf("\tqcsamunlock [PORT]\n");
	printf("\tqcsamunlock\\\\.\\COM10\n");
	printf("\tqcsamunlock /dev/ttyUSB0\n");
	printf("---------------------------------------------------\n");
	printf(" Copyright 2015 Gassan Idriss <ghassani@gmail.com> \n");
	printf(" This software is distributed with OpenPST.        \n");
	printf(" See http://www.github.com/ghassani/openpst        \n");	
	printf(" This software is free and licensed under GPL.     \n");
	printf("---------------------------------------------------\n");
}

/**
* @brief processItem - Delete an NV item by deltree traversal exploit
* @param int item - The NV item number to delete 
* @return bool
*/
bool processItem(int item)
{
	ostringstream path; 	
	path << "/public/../nvm/num/" << item; // deltree only works in /public

	if (efsManager.mkdir("public", 0x00) == efsManager.kDmEfsSuccess) {
		if (efsManager.deltree(path.str()) == efsManager.kDmEfsSuccess) {
			return true;
		} else {
			printf("Error removing nv item %d - Path: %s\n", item, path.str().c_str());
			return false;
		}
	}

	return false;
}

/**
* @brief main - Program entry point
* @param int argc
* @param char **argv
* @return int
*/
int main(int argc, char **argv) {
	QcdmEfsStatfsResponse 		 statResponse;
	QcdmEfsSyncResponse 		 syncResponse;
	QcdmEfsGetSyncStatusResponse syncStatusResponse;
	int syncMaxRetries			 = 10;
	int syncRetries			 	 = 0;

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

	if (efsManager.statfs("/", statResponse) == efsManager.kDmEfsSuccess) {
		
		if (!processItem(10080) || !processItem(10074) || !processItem(10073)) {
			printf("Operation Failed!\n");
			port.close();
			return 1;
		}
		
	} else {
		printf("Error checking for EFS access\n");
		port.close();
		return 1;
	}

	try {
		
		syncResponse = efsManager.syncNoWait("/");

		while (syncMaxRetries > syncRetries) {
			
			syncStatusResponse = efsManager.getSyncStatus(syncResponse.token);

			if (syncStatusResponse.status) {
				printf("Sync Complete\n");
				printf("Operation Successful. Reboot device and insert a different carriers SIM.\n");
				port.close();
				return 0;
			} else {
				sleep(1000); // wait and check again
				syncRetries++;
			}
		}

		printf("Sync Error. Device may still have been unlocked. Reboot and insert a different carriers SIM.\n");

	} catch (std::exception e) {
		printf("Error encountered during sync: %s\n", e.what());
		return 1;
	}

    return 0;
}
