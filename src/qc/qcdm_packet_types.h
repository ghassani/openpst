/**
* LICENSE PLACEHOLDER
*
* @file diag.h
* @package OpenPST
* @brief QCDM definitions and structures
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author Matteson Raab <mraabhimself@gmail.com>
*/

#ifndef _QC_QCDM_PACKET_TYPES_H
#define _QC_QCDM_PACKET_TYPES_H

#include "include/definitions.h"
#include "qc/diag.h"

PACKED(typedef struct qcdm_phone_mode_tx_t{
    uint8_t command = DIAG_CONTROL_F;
    uint8_t mode;
    uint8_t padding = 0x0;
}) qcdm_phone_mode_tx_t;

PACKED(typedef struct qcdm_phone_mode_rx_t{
    uint8_t command;
    uint8_t status;
}) qcdm_phone_mode_rx_t;

PACKED(typedef struct qcdm_spc_tx_t{
    uint8_t command = DIAG_SPC_F;
    uint8_t spc[6];
}) qcdm_spc_tx_t;

PACKED(typedef struct qcdm_spc_rx_t{
    uint8_t command;
    uint8_t status;
}) qcdm_spc_rx_t;

PACKED(typedef struct qcdm_16pw_tx_t{
    uint8_t command = DIAG_PASSWORD_F;
    uint8_t password[8];
}) qcdm_16pw_tx_t;

PACKED(typedef struct qcdm_16pw_rx_t{
    uint8_t command;
    uint8_t status;
}) qcdm_16pw_rx_t;

PACKED(typedef struct qcdm_nv_tx_t{
    uint8_t cmd;
    uint16_t nvItem;
    uint8_t data[DIAG_NV_ITEM_SIZE] = { 0x0 };
}) qcdm_nv_tx_t;

PACKED(typedef struct qcdm_nv_alt_tx_t{
    uint8_t padding = 0x0;
    uint8_t data[DIAG_NV_ITEM_SIZE - 1] = { 0x0 };
}) qcdm_nv_alt_tx_t;

PACKED(typedef struct qcdm_nv_alt2_tx_t{
    uint16_t padding = 0x0;
    uint8_t data[DIAG_NV_ITEM_SIZE - 2] = { 0x0 };
}) qcdm_nv_alt2_tx_t;

PACKED(typedef struct qcdm_nv_raw_tx_t{
    uint8_t data[DIAG_NV_ITEM_SIZE - 3] = { 0x0 };
}) qcdm_nv_raw_tx_t;

PACKED(typedef struct qcdm_nv_rx_t{
    uint8_t cmd;
    uint16_t nvItem;
    uint8_t data[DIAG_NV_ITEM_SIZE];
}) qcdm_nv_rx_t;

PACKED(typedef struct qcdm_nv_alt_rx_t{
    uint8_t cmd;
    uint16_t nvItem;
    uint8_t padding;
    uint8_t data[DIAG_NV_ITEM_SIZE - 1];
}) qcdm_nv_alt_rx_t;

PACKED(typedef struct qcdm_nv_alt2_rx_t{
    uint8_t cmd;
    uint16_t nvItem;
    uint8_t padding[2];
    uint8_t data[DIAG_NV_ITEM_SIZE -2];
}) qcdm_nv_alt2_rx_t;

#endif // _QC_QCDM_PACKET_TYPES_H
