/**
* LICENSE PLACEHOLDER
*
* @file main.c
* @package OpenPST Extras - QFPROM Kernel Module
* @brief This linux kernel module allows you to read/write QFPROM rows
*			where permission is granted. Optionally can run as a tcp
*			server so you can call to it from outside of kernel space.
*			@see client.py for connecting to the server
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#include <linux/kernel.h>
#include <linux/module.h>
#include "qfprom.h"
#include "server.h"
#include "debug.h"

typedef struct qfprom_read_operation_entry_s {
	char name[50];
	uint32_t address;
	bool corrected;
} qfprom_read_operation_entry_t;

typedef struct qfprom_write_operation_entry_s {
	char name[50];
	uint32_t address;
	uint32_t lsb;
	uint32_t msb;
	uint32_t bus_clk_khz;
} qfprom_write_operation_entry_t;

static short int start_tcp = 0;


static qfprom_read_operation_entry_t read_rows_table[] = {
	{ "QFPROM_SECURE_BOOT_ENABLE", 	0xFC4B83F8, false },
};

static qfprom_write_operation_entry_t write_rows_table[] = {
	// Enter the rows you wish to add to the write list
	//   name   address     lsb         msb         bus_clk_khz
	//{ "Name", 0x80000000, 0x00000000, 0x00000000, 0x00000000 },
};

static int __init tz_qfprom_init(void)
{
	int i;
	int read_count  = sizeof(read_rows_table)/sizeof(qfprom_read_operation_entry_t);
	int write_count = sizeof(write_rows_table)/sizeof(qfprom_write_operation_entry_t);
	scm_qfprom_read_row_data_t read_row_data = {};
	scm_qfprom_write_row_data_t write_row_data = {};

	if (start_tcp > 0) {
		tcp_server_start(start_tcp);
	}

	if (sizeof(read_rows_table) > 0) {
		for (i=0; i < read_count; i++) {
			tz_qfprom_read_row(read_rows_table[i].address, (read_rows_table[i].corrected ? 0x01 : 0x00), &read_row_data);
		}
	}

	if (sizeof(write_rows_table) > 0) {
		for (i=0; i < write_count; i++) {
			tz_qfprom_write_row(write_rows_table[i].address, write_rows_table[i].lsb, write_rows_table[i].msb, write_rows_table[i].bus_clk_khz, &write_row_data);
		}
	}
	
    return 0;
}

static void __exit tz_qfprom_exit(void)
{
	if (start_tcp > 0) {
		tcp_server_shutdown();
	}
}

module_init(tz_qfprom_init);
module_exit(tz_qfprom_exit);

MODULE_DESCRIPTION("QFPROM");
MODULE_AUTHOR("Gassan Idriss <ghassani@gmail.com>");
MODULE_LICENSE("GPL");

module_param(start_tcp, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(start_tcp, "Set to port you wish to enable the tcp server on. By default this is not enabled.");
