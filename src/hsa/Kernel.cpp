#include <hsa/Kernel.h>
#include <hsa/Context.h>
#include <hsa/Exception.h>
#include <cstring>

using namespace hsa;

/*
 * Determines if a memory region can be used for kernarg
 * allocations.
 */
static hsa_status_t get_kernarg(hsa_region_t region, void* data) {
    hsa_region_flag_t flags;
    hsa_region_get_info(region, HSA_REGION_INFO_FLAGS, &flags);
    if (flags & HSA_REGION_FLAG_KERNARG) {
        hsa_region_t* ret = (hsa_region_t*) data;
        *ret = region;
        return HSA_STATUS_SUCCESS;
    }
    return HSA_STATUS_SUCCESS;
}

Kernel::Kernel(Context *context, hsa_ext_code_descriptor_t *hsaCodeDescriptor)
	: _context(context), _hsaCodeDescriptor(hsaCodeDescriptor), _kernel_arg_buffer(nullptr)
{
    /*
     * Find a memory region that supports kernel arguments.
     */
    hsa_region_t kernarg_region = 0;
    hsa_agent_iterate_regions(_context->getHsaAgent(), get_kernarg, &kernarg_region);
    hsa_status_t err = (kernarg_region == 0) ? HSA_STATUS_ERROR : HSA_STATUS_SUCCESS;
    CHECK_HSA(Finding a kernarg memory region, err);
	
	size_t kernel_arg_buffer_size = hsaCodeDescriptor->kernarg_segment_byte_size;
    err = hsa_memory_allocate(kernarg_region, kernel_arg_buffer_size,
                        &_kernel_arg_buffer);
	CHECK_HSA(Allocating kernel argument memory buffer, err);
	
	_argPtr = _kernel_arg_buffer;
}

Kernel::~Kernel()
{
	if(_kernel_arg_buffer!=nullptr)
		hsa_memory_free(_kernel_arg_buffer);
}

Kernel& Kernel::operator << (void *val) 
{		
	memcpy(_argPtr, val, sizeof(void*));
	_argPtr+=sizeof(void *);
	return *this;
}

/*Kernel& Kernel::operator << (hsa::RegisterMemory& memory)
{
	*_argPtr = memory.ptr();
	_argPtr+=sizeof(void *);
	return *this;
}*/

int Kernel::status() const
{
	return _status;
}
