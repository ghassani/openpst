/**
* LICENSE PLACEHOLDER
*
* @file mbn.h
* @package OpenPST
* @brief MBN file format definitions and structures
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifndef _QC_MBN_H
#define _QC_MBN_H

#include "include/definitions.h"

#define MBN_HEADER_MAX_SIZE 80
#define MBN_HEADER_MIN_SIZE 40
#define MBN_EIGHTY_BYTE_MAGIC 0x73D71034

PACKED(typedef struct {
    uint32_t  codeword;            /* Codeword/magic number defining flash type
                                information. */
    uint32_t  magic;               /* Magic number */
    uint32_t  image_id;        /* image content */
    uint32_t  reserved1;          /* RESERVED */
    uint32_t  reserved2;          /* RESERVED */
    uint32_t  image_src;             /* Location of RPM_SBL in flash or e-hostdl in RAM. This is given in
                                byte offset from beginning of flash/RAM.  */
    uint32_t image_dest_ptr;        /* Pointer to location to store RPM_SBL/e-hostdl in RAM.
                                Also, entry point at which execution begins.
                                */
    uint32_t  image_size;      /* Size of RPM_SBL image in bytes */
    uint32_t  code_size;       /* Size of code region in RPM_SBL image in bytes */
    uint32_t signature_ptr;         /* Pointer to images attestation signature */
    uint32_t  signature_size;        /* Size of the attestation signature in
                                bytes */
    uint32_t cert_chain_ptr;  /* Pointer to the chain of attestation
                                certificates associated with the image. */
    uint32_t  cert_chain_size; /* Size of the attestation chain in bytes */

    uint32_t  oem_root_cert_sel;  /* Root certificate to use for authentication.
                                Only used if SECURE_BOOT1 table_sel fuse is
                                OEM_TABLE. 1 indicates the first root
                                certificate in the chain should be used, etc */
    uint32_t  oem_num_root_certs; /* Number of root certificates in image.
                                Only used if SECURE_BOOT1 table_sel fuse is
                                OEM_TABLE. Denotes the number of certificates
                                OEM has provisioned                          */

    uint32_t  reserved3;          /* RESERVED */
    uint32_t  reserved4;          /* RESERVED */
    uint32_t  reserved5;          /* RESERVED */
    uint32_t  reserved6;          /* RESERVED */
    uint32_t  reserved7;          /* RESERVED */
}) eighty_byte_mbn_header_t;

PACKED(typedef struct {
  uint32_t image_id;           /**< Identifies the type of image this header
                                  represents (OEM SBL, AMSS, Apps boot loader,
                                  etc.). */
  uint32_t header_vsn_num;     /**< Header version number. */
  uint32_t image_src;          /**< Offset from end of the Boot header where the
                                  image starts. */
  uint8_t* image_dest_ptr;     /**< Pointer to location to store image in RAM.
                                  Also, entry point at which image execution
                                  begins. */
  uint32_t image_size;         /**< Size of complete image in bytes */
  uint32_t code_size;          /**< Size of code region of image in bytes */
  uint8_t* signature_ptr;      /**< Pointer to images attestation signature */
  uint32_t signature_size;     /**< Size of the attestation signature in
                                 bytes */
  uint8_t* cert_chain_ptr;     /**< Pointer to the chain of attestation
                                 certificates associated with the image. */
  uint32_t cert_chain_size;    /**< Size of the attestation chain in bytes */
}) fourty_byte_mbn_header_t;

enum MBN_IMAGE {
    MBN_IMAGE_NONE           = 0x00,
    MBN_IMAGE_OEM_SBL_IMG    = 0x01,
    MBN_IMAGE_AMSS_IMG       = 0x02,
    MBN_IMAGE_QCSBL_IMG      = 0x03,
    MBN_IMAGE_HASH_IMG       = 0x04,
    MBN_IMAGE_APPSBL_IMG     = 0x05,
    MBN_IMAGE_APPS           = 0x06,
    MBN_IMAGE_HOSTDL         = 0x07,
    MBN_IMAGE_DSP1           = 0x08,
    MBN_IMAGE_FSBL           = 0x09,
    MBN_IMAGE_DBL            = 0x0A,
    MBN_IMAGE_OSBL           = 0x0B,
    MBN_IMAGE_DSP2           = 0x0C,
    MBN_IMAGE_EHOSTDL        = 0x0D,
    MBN_IMAGE_NANDPRG        = 0x0E,
    MBN_IMAGE_NORPRG         = 0x0F,
    MBN_IMAGE_RAMFS1         = 0x10,
    MBN_IMAGE_RAMFS2         = 0x11,
    MBN_IMAGE_ADSP_Q5        = 0x12,
    MBN_IMAGE_APPS_KERNEL    = 0x13,
    MBN_IMAGE_BACKUP_RAMFS   = 0x14,
    MBN_IMAGE_SBL1           = 0x15,
    MBN_IMAGE_SBL2           = 0x16,
    MBN_IMAGE_RPM            = 0x17,
    MBN_IMAGE_SBL3           = 0x18,
    MBN_IMAGE_TZ             = 0x19,
    MBN_IMAGE_SSD_KEYS       = 0x1A,
    MBN_IMAGE_GEN            = 0x1B,
    MBN_IMAGE_DSP3           = 0x1C,
    MBN_IMAGE_ACDB           = 0x1D,
    MBN_IMAGE_WDT            = 0x1E,
    MBN_IMAGE_MBA            = 0x1F,
};
#endif // _QC_MBN_H
