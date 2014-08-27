#ifndef _ELF_UTILS_H_
#define _ELF_UTILS_H_

#include "hsa_ext_finalize.h"

#ifdef __cplusplus
extern "C" {
#endif 

typedef enum status {
    STATUS_SUCCESS=0,
    STATUS_KERNEL_INVALID_SECTION_HEADER=1,
    STATUS_KERNEL_ELF_INITIALIZATION_FAILED=2,
    STATUS_KERNEL_INVALID_ELF_CONTAINER=3,
    STATUS_KERNEL_MISSING_DATA_SECTION=4,
    STATUS_KERNEL_MISSING_CODE_SECTION=5,
    STATUS_KERNEL_MISSING_OPERAND_SECTION=6,
    STATUS_UNKNOWN=7,
} status_t;

status_t create_brig_module_from_brig_file(const char* file_name, hsa_ext_brig_module_t** brig_module);

void destroy_brig_module(hsa_ext_brig_module_t* brig_module);

#ifdef __cplusplus
}
#endif

#endif