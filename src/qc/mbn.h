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
#define MBN_ROOT_CERTIFICATE_SIZE 1024 * 6
#define MBN_MAX_ROOT_CERTIFICATES 4

enum MBN_IMAGE_SEGMENTS {
  MBN_SEGMENT_HEADER = 1,
  MBN_SEGMENT_CODE,
  MBN_SEGMENT_SIGNATURE,
  MBN_SEGMENT_X509_CHAIN_CERTIFICATE
};

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
}) MbnSblHeader;

PACKED(typedef struct {
  uint32_t image_id;           /**< Identifies the type of image this header
                                  represents (OEM SBL, AMSS, Apps boot loader,
                                  etc.). */
  uint32_t header_vsn_num;     /**< Header version number. */
  uint32_t image_src;          /**< Offset from end of the Boot header where the
                                  image starts. */
  uint32_t image_dest_ptr;     /**< Pointer to location to store image in RAM.
                                  Also, ent676ry point at which image execution
                                  begins. */
  uint32_t image_size;         /**< Size of complete image in bytes */
  uint32_t code_size;          /**< Size of code region of image in bytes */
  uint32_t signature_ptr;      /**< Pointer to images attestation signature */
  uint32_t signature_size;     /**< Size of the attestation signature in
                                 bytes */
  uint32_t cert_chain_ptr;     /**< Pointer to the chain of attestation
                                 certificates associated with the image. */
  uint32_t cert_chain_size;    /**< Size of the attestation chain in bytes */
}) MbnHeader;

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
  MBN_IMAGE_LAST
};



/**
  ASN1 Constants and definitions
*/
#define SECASN1_NO_TYPE_CHECK         (0x00)
#define SECASN1_bool_TYPE          (0x01)
#define SECASN1_INTEGER_TYPE          (0x02)
#define SECASN1_BIT_STRING_TYPE       (0x03)
#define SECASN1_OCTET_STRING_TYPE     (0x04)
#define SECASN1_NULL_TYPE             (0x05)
#define SECASN1_OID_TYPE              (0x06)
#define SECASN1_UTF8_STRING_TYPE      (0x0c)
#define SECASN1_SEQUENCE_TYPE         (0x10)
#define SECASN1_SET_TYPE              (0x11)
#define SECASN1_PRINTABLE_STRING_TYPE (0x13)
#define SECASN1_TELETEX_STRING_TYPE   (0x14)
#define SECASN1_UTC_TYPE              (0x17)
/*!
  @}
*/
/*--------------------------------------------------------------------------
                Modular Constants and definitions
--------------------------------------------------------------------------*/

/**
  ASN.1 Error Codes
*/
typedef enum
{
  E_ASN1_SUCCESS = 0,
  E_ASN1_INVALID_TAG,
  E_ASN1_NO_DATA,
  E_ASN1_INVALID_DATA,
  E_ASN1_INVALID_ARG
} secasn1_err_type;

/**
  ASN.1 data holder
*/
typedef struct
{
  uint8_t *data;
  uint16_t len;
} secasn1_data_type;

/**
  ASN.1 bit string data holder
*/
typedef struct
{
  uint8_t *data;
  uint16_t len;
  uint8_t unused;
} secasn1_bit_string_type;



/* Key Usage Masks */
#define SECX509_KEY_USAGE_DIG_SIG   (0x0100) /* digital signature */
#define SECX509_KEY_USAGE_NON_REP   (0x0080) /* non-repudiation   */
#define SECX509_KEY_USAGE_KEY_ENC   (0x0040) /* key encipherment  */
#define SECX509_KEY_USAGE_DAT_ENC   (0x0020) /* data encipherment */
#define SECX509_KEY_USAGE_KEY_ARG   (0x0010) /* key agreement     */
#define SECX509_KEY_USAGE_KEY_CRT   (0x0008) /* key cert sign     */
#define SECX509_KEY_USAGE_CRL_SIG   (0x0004) /* CRL sign          */
#define SECX509_KEY_USAGE_ENC_OLY   (0x0002) /* encipher only     */
#define SECX509_KEY_USAGE_DEC_OLY   (0x0001) /* decipher only     */
/* Extended Key Usage Masks */
#define SECX509_EX_KEY_USAGE_SAUTH  (0x0001) /* TLS Web Server Authentication*/
#define SECX509_EX_KEY_USAGE_CAUTH  (0x0002) /* TLS Web Client Authentication*/
#define SECX509_EX_KEY_USAGE_CODE   (0x0004) /* Downloadable Code Signing    */
#define SECX509_EX_KEY_USAGE_EMAIL  (0x0008) /* Email Protection             */
#define SECX509_EX_KEY_USAGE_TIME   (0x0010) /* Time Stamping                */
#define SECX509_EX_KEY_USAGE_SGC    (0x0020) /* Secured Gated Crypto         */

/*==========================================================================

                       Type Definitions

==========================================================================*/
/* Public Key algorithms in the certificate */
typedef enum
{
  SECX509_PUBKEY_RSA = 0,
  SECX509_PUBKEY_DSA = 1,
  SECX509_PUBKEY_DH  = 2,
  SECX509_PUBKEY_MAX,               /* Last one, for error checking */
  SECX509_RESERVED_1 = 0x7FFFFFFF
} pbl_secx509_pubkey_algo_type;

/* Certificate signature algorithm type */
typedef enum
{
  SECX509_md5WithRSAEncryption    = 0,
  SECX509_md2WithRSAEncryption    = 1,
  SECX509_sha1WithRSAEncryption   = 2,
  SECX509_sha256WithRSAEncryption = 3,
  SECX509_SIG_ALGO_MAX,                 /* Last one, for error checking */
  SECX509_RESERVED_2            = 0x7FFFFFFF
} pbl_secx509_sig_algo_type;


/* RSA public key parameters */
typedef struct pbl_secx509_rsa_pubkey_type
{
  uint32_t  mod_len;
  const uint8_t   *mod_data;
  uint32_t  exp_e_len;
  const uint8_t   *exp_e_data;

} pbl_secx509_rsa_pubkey_type;

/* Union of all the public key types */
typedef struct pbl_secx509_pubkey_type
{
  pbl_secx509_pubkey_algo_type  algo;
  union
  {
    pbl_secx509_rsa_pubkey_type  rsa;
  }key;

} pbl_secx509_pubkey_type;

/* Signature Structure */
typedef struct pbl_secx509_signature_type
{
  pbl_secx509_sig_algo_type   algo_id;
  secasn1_data_type           val;

} pbl_secx509_signature_type;

/* Distinguished name structure */
typedef struct pbl_secx509_dn_type
{
  uint32_t             num_attrib;
  secasn1_data_type  data;

} pbl_secx509_dn_type;

/* Version structure */
typedef struct pbl_secx509_version_type
{
  uint32_t             ver;
  secasn1_data_type  val;

} pbl_secx509_version_type;

/* Time structure */
typedef struct pbl_secx509_time_type
{
  uint32_t             time;
  secasn1_data_type  data;

} pbl_secx509_time_type;

/* Authority Key Identifier structure */
typedef struct pbl_secx509_auth_key_id_type
{
  bool            set;
  secasn1_data_type  key_id;
  pbl_secx509_dn_type    name;
  secasn1_data_type  serial_number;

} pbl_secx509_auth_key_id_type;

/* Subject Key Identifier structure */
typedef struct pbl_secx509_subject_key_id_type
{
  bool            set;
  secasn1_data_type  key_id;

} pbl_secx509_subject_key_id_type;

/* Key Usage structure */
typedef struct pbl_secx509_key_usage_type
{
  uint32_t   val;
  bool  set;

} pbl_secx509_key_usage_type;

/* CA structure */
typedef struct pbl_secx509_ca_type
{
  bool  set;
  bool  val;

} pbl_secx509_ca_type;

/* Extension structure type */
typedef struct pbl_secx509_ext_type
{
  bool                          set;
  pbl_secx509_auth_key_id_type     auth_key_id;
  pbl_secx509_subject_key_id_type  subject_key_id;
  pbl_secx509_key_usage_type       key_usage;
  pbl_secx509_key_usage_type       ex_key_usage;
  int32_t                            path_len;
  pbl_secx509_ca_type              ca;

} pbl_secx509_ext_type;

/* Certificate information structure */
typedef struct pbl_secx509_cert_info_type
{
  pbl_secx509_version_type     version;
  secasn1_data_type            serial_number;
  pbl_secx509_signature_type   algorithm;
  pbl_secx509_dn_type          issuer;
  pbl_secx509_time_type        not_before;
  pbl_secx509_time_type        not_after;
  pbl_secx509_dn_type          subject;
  secasn1_bit_string_type      issuer_unique_id;
  secasn1_bit_string_type      subject_unique_id;
  pbl_secx509_ext_type         extension;

} pbl_secx509_cert_info_type;

/* Certificate structure */
typedef struct pbl_secx509_cert_type
{
  /* The cert_info needs to be the first member */
  pbl_secx509_cert_info_type  cert_info;

  uint32_t                      cinf_offset; //where the certificate actually starts -
                                           //after the initial tag/len
  uint32_t                      cinf_byte_len; //length of where the certificate actually starts
                                             //upto (but not including) the certificate signature
  uint32_t                      asn1_size_in_bytes; //size of the entire certificate (including the initial tag/len)

  /* Signature info on the cert */
  pbl_secx509_pubkey_type     pkey;
  pbl_secx509_sig_algo_type   sig_algo;
  const uint8_t                *sig;
  uint32_t                      sig_len;

  /*For verification */
  uint8_t                       cert_hash[32];
} pbl_secx509_cert_type;



/* Certificate list struct */
typedef struct pbl_secx509_cert_list_struct
{
  pbl_secx509_cert_type  cert[3];
  uint32_t               size;

} pbl_secx509_cert_list_type;

/* certificate list context type */
typedef struct
{
  uint32_t                     purpose;
  uint32_t                     trust;
  uint32_t                     depth;
  pbl_secx509_cert_list_type*  ca_list;

} pbl_secx509_cert_ctx_type;


typedef enum
{
  E_X509_SUCCESS = 0,
  E_X509_FAILURE,
  E_X509_NO_DATA,
  E_X509_DATA_INVALID,
  E_X509_BAD_DATA,
  E_X509_DATA_TOO_LARGE,
  E_X509_DATA_EXPIRED,
  E_X509_NO_MEMORY,
  E_X509_INVALID_ARG,
  E_X509_NOT_SUPPORTED,
  E_X509_RESERVED       = 0x7FFFFFFF
} secx509_errno_enum_type;

typedef enum
{
  E_X509_CODE_HASH_NOT_SPECIFIED = 0,
  E_X509_CODE_HASH_SHA1,
  E_X509_CODE_HASH_SHA256,
  E_X509_CODE_HASH_RESERVED       = 0x7FFFFFFF
}secx509_code_hash_algo_type;

typedef struct secx509_ou_field_info_type
{
  uint64_t                      debug_enable;
  uint64_t                      sw_id;
  uint64_t                      hw_id;
  secx509_code_hash_algo_type code_hash_algo;
  uint64_t                      crash_dump_enable;
} secx509_ou_field_info_type;


#endif // _QC_MBN_H
