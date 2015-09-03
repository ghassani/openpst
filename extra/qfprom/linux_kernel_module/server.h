/**
* LICENSE PLACEHOLDER
*
* @file server.h
* @package OpenPST Extras - QFPROM Kernel Module
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#ifndef __QFPROM_TCP_SERVER_H
#define __QFPROM_TCP_SERVER_H

#include <linux/kernel.h>
#include <linux/kthread.h>
#include <net/sock.h>
#include <net/tcp.h>
#include "qfprom.h"


enum QFPROM_TCP_REQUEST {
    QFPROM_TCP_DISCONNECT_REQUEST   = 0,
    QFPROM_TCP_READ_REQUEST         = 1,
    QFPROM_TCP_WRITE_REQUEST        = 2
};

/**
* QFPROM request and response structures
*/
typedef struct qfprom_tcp_read_row_req_s {
    uint32_t address;
    uint32_t type;
} __attribute__ ((packed)) qfprom_tcp_read_row_req_t;

typedef struct qfprom_tcp_read_row_resp_s {
    uint32_t address;
    uint32_t type;
    uint32_t lsb;
    uint32_t msb;
    uint32_t error;
} __attribute__ ((packed)) qfprom_tcp_read_row_resp_t;

typedef struct qfprom_tcp_write_row_req_s {
    uint32_t address;
    uint32_t lsb;
    uint32_t msb;
    uint32_t bus_clk_khz;
} __attribute__ ((packed)) qfprom_tcp_write_row_req_t;

typedef struct qfprom_tcp_write_row_resp_s {
    uint32_t address;
    uint32_t lsb;
    uint32_t msb;
    uint32_t bus_clk_khz;
    uint32_t error;
} __attribute__ ((packed)) qfprom_tcp_write_row_resp_t;

typedef struct qfprom_tcp_req_s {
    int8_t command;
    union {
        qfprom_tcp_read_row_req_t read;
        qfprom_tcp_write_row_req_t write;
    };
} __attribute__ ((packed)) qfprom_tcp_req_t;

typedef struct qfprom_tcp_resp_s {
    int8_t command;
    union {
        qfprom_tcp_read_row_resp_t read;
        qfprom_tcp_write_row_resp_t write;
    };
} __attribute__ ((packed)) qfprom_tcp_resp_t;

int tcp_server_start(int port);
void tcp_server_shutdown(void);
int tcp_server_thread(void);
int tcp_server_receive(struct socket* sock, uint8_t* buffer, int length);
int tcp_server_send(struct socket* sock, uint8_t* data, int length);

#endif // __QFPROM_TCP_SERVER_H




