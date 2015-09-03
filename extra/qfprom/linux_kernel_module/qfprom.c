/**
* LICENSE PLACEHOLDER
*
* @file qfprom.ch
* @package OpenPST Extras - QFPROM Kernel Module
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#include "qfprom.h"

int tz_qfprom_read_row(uint32_t address, uint32_t type, scm_qfprom_read_row_data_t* row_data)
{
    int ret = 0;
    scm_qfprom_read_row_req_t req           = {};
    scm_qfprom_read_row_resp_t resp         = {};
    struct ion_client *ion_clientp          = NULL;
    struct ion_handle *ion_row_data_handlep = NULL;
    uint32_t row_data_phyaddr_len           = 0;
    ion_phys_addr_t row_data_phyaddr        = 0;
    scm_qfprom_read_row_data_t* row_datap;

    ion_clientp = msm_ion_client_create(UINT_MAX, "tz_qfprom_read_row");

    if (IS_ERR_OR_NULL(ion_clientp)) {      
        log("Error creating ion client\n");
        return -EINVAL;
    }

    ion_row_data_handlep = ion_alloc(ion_clientp, sizeof(scm_qfprom_read_row_data_t), 
        sizeof(scm_qfprom_read_row_data_t), ION_HEAP(ION_QSECOM_HEAP_ID), 0);


    if (IS_ERR_OR_NULL(ion_row_data_handlep)) {
        log("Ion client could not retrieve the handle\n");
        goto error;
    }

    if (ion_phys(ion_clientp, ion_row_data_handlep, &row_data_phyaddr, &row_data_phyaddr_len)) {
        log("Ion conversion to physical address failed\n");
        goto error;
    }

    req.address     = address;
    req.type        = type;
    req.row_data    = row_data_phyaddr;
    req.error       = row_data_phyaddr + (sizeof(scm_qfprom_read_row_data_t) - sizeof(uint32_t));

    ret = scm_call(TZ_SVC_FUSE, TZ_QFPROM_READ_ROW_ID, &req, sizeof(req), &resp, sizeof(resp));

    if (ret) {
        
        log("Reading row at 0x%08X faile. SCM call failed. %d\n", address, ret);
        
        row_data->lsb   = 0;
        row_data->msb   = 0;    
        row_data->error = ret;  

    } else {

        row_datap = (scm_qfprom_read_row_data_t*)ion_map_kernel(ion_clientp, ion_row_data_handlep);
        
        if (IS_ERR_OR_NULL(row_datap)) {
            log("Ion memory mapping failed\n");
            goto error;
        }

        if (row_datap->error) {             

            log("Error reading row at 0x%08X. Error: %d\n", address, row_datap->error);

        } else {
            log("Row data at 0x%08X - LSB: 0x%08X - MSB: 0x%08X\n", address, row_datap->lsb, row_datap->msb);
        }

        row_data->lsb   = row_datap->lsb;
        row_data->msb   = row_datap->msb;   
        row_data->error = row_datap->error; 
    }

    ion_free(ion_clientp, ion_row_data_handlep);
    ion_client_destroy(ion_clientp);    
    return ret; 
error:
    ion_free(ion_clientp, ion_row_data_handlep);
    ion_client_destroy(ion_clientp);
    return -EINVAL;
}


int tz_qfprom_write_row(uint32_t address, uint32_t lsb, uint32_t msb, uint32_t bus_clk_khz, scm_qfprom_write_row_data_t* row_data)
{
    int ret = 0;
    scm_qfprom_write_row_req_t req          = {};
    scm_qfprom_write_row_resp_t resp        = {};
    struct ion_client *ion_clientp          = NULL;
    struct ion_handle *ion_row_data_handlep = NULL;
    uint32_t row_data_phyaddr_len           = 0;
    ion_phys_addr_t row_data_phyaddr        = 0;
    scm_qfprom_write_row_data_t* row_datap;

    ion_clientp = msm_ion_client_create(UINT_MAX, "tz_qfprom_write_row");

    if (IS_ERR_OR_NULL(ion_clientp)) {      
        log("Error creating ion client\n");
        return -EINVAL;
    }

    ion_row_data_handlep = ion_alloc(ion_clientp, sizeof(scm_qfprom_write_row_data_t), 
        sizeof(scm_qfprom_write_row_data_t), ION_HEAP(ION_QSECOM_HEAP_ID), 0);


    if (IS_ERR_OR_NULL(ion_row_data_handlep)) {
        log("Ion client could not retrieve the handle\n");
        goto error;
    }

    if (ion_phys(ion_clientp, ion_row_data_handlep, &row_data_phyaddr, &row_data_phyaddr_len)) {
        log("Ion conversion to physical address failed\n");
        goto error;
    }
    
    row_datap = (scm_qfprom_write_row_data_t*)ion_map_kernel(ion_clientp, ion_row_data_handlep);
    
    if (IS_ERR_OR_NULL(row_datap)) {
        log("Ion memory mapping failed\n");
        goto error;
    }

    row_datap->lsb = lsb;
    row_datap->msb = msb;

    req.address     = address;
    req.bus_clk_khz = bus_clk_khz;
    req.row_data    = row_data_phyaddr;
    req.error       = row_data_phyaddr + (sizeof(scm_qfprom_write_row_data_t) - sizeof(uint32_t));

    ret = scm_call(TZ_SVC_FUSE, TZ_QFPROM_WRITE_ROW_ID, &req, sizeof(req), &resp, sizeof(resp));

    if (ret) {
        
        log("Writing row at 0x%08X faile. SCM call failed. %d\n", address, ret);
        
        row_data->lsb   = 0;
        row_data->msb   = 0;    
        row_data->error = ret;  

    } else {

        if (row_datap->error) {             

            log("Error writing row at 0x%08X. Error: %d\n", address, row_datap->error);

        } else {
            log("Row data at 0x%08X - LSB: 0x%08X - MSB: 0x%08X\n", address, row_datap->lsb, row_datap->msb);
        }

        row_data->lsb   = row_datap->lsb;
        row_data->msb   = row_datap->msb;   
        row_data->error = row_datap->error; 
    }

    ion_free(ion_clientp, ion_row_data_handlep);
    ion_client_destroy(ion_clientp);    
    return ret; 
error:
    ion_free(ion_clientp, ion_row_data_handlep);
    ion_client_destroy(ion_clientp);
    return -EINVAL;
}
