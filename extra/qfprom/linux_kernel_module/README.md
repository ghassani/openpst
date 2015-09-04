QFPROM Kernel Module
===================
This kernel module allows for QFPROM reading and writing through either a pre-defined static list within the code or through a TCP server from userland or remotely. This module uses SCM calls to read and write to fuses.

You still need read/write permissions defined in certain fuses to be able to read/write to certain rows.

You will need to compile the module for your kernel. There are many instructions on the web that can be of assistance here.

##Read/Write from a static list defined within the module
You can add new rows to read/write from in the module itself. Open up `main.`c and find the fuses static list of addresses. See the structures `qfprom_write_operation_entry_t` and `qfprom_write_operation_entry_t` for formatting.

Once compiled just install teh module, it will read and write the specified rows
    
    insmod qfprom_tcp.ko
    
##Read/Write through a TCP Server

To enable the TCP server, install the module passing the port with the parameter `start_tcp`

    insmod qfprom_tcp.ko start_tcp=[PORT]
    insmod qfprom_tcp.ko start_tcp=610

You can see in your kernel logs that the TCP server was successfully started

    cat /proc/kmsg | grep qfprom

##Client

The provided client is written in python. You could write your own client if you need, request and response structures are defined in server.h

To use the client, first install the server module. Then from the command line execute the following command:

    python client.py [HOST] [PORT] -r 0x08000000 0x07000000

This would connect and read the rows at address 0x08000000 and 0x07000000.

You can also request corrected reads for rows that are corrected:

    python client.py [HOST] [PORT] -rc 0x08000000 0x07000000

You can combine the two as well:

    python client.py [HOST] [PORT] -r 0x08000000 0x07000000 -rc 0x08000000 0x07000000

##Read / Write Permissions

You still need proper permissions blown into certain fuses to read/write certain rows.
