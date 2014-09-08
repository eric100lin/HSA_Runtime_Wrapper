#include <hsa/Kernel.h>
#include <hsa/Context.h>
#include <hsa/Exception.h>
#include <hsa/RegisterMemory.h>
#include <hsa/CommandQueue.h>
#include <cstring>

#if defined(__APPLE__) || defined(MACOSX)
    #include <OpenCL/cl.h>
#else
    #include <CL/cl.h>
#endif

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
	: _context(context),  _kernel_arg_buffer(nullptr), _hsaCodeDescriptor(hsaCodeDescriptor)
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
}

Kernel::~Kernel()
{
	if(_kernel_arg_buffer!=nullptr)
		hsa_memory_free(_kernel_arg_buffer);
}

uint64_t Kernel::getKernArgAddress() const
{
	return (uint64_t)_kernel_arg_buffer;
}

uint64_t Kernel::getExtCodeHandle() const
{
	return _hsaCodeDescriptor->code.handle;
}

void Kernel::setKernelArg(unsigned int offset, size_t size, const void *argument)
{
	void *ptrArg = _kernel_arg_buffer+offset;
	memcpy(ptrArg, argument, size);
}

KernelArgs::KernelArgs(hsa::Kernel *kernel) : _argOffset(0), _kernel(kernel)
{
}

KernelArgs& KernelArgs::operator << (enum Alignment alignment)
{
	_argOffset += alignment;
	return *this;
}

KernelArgs& KernelArgs::operator << (const char * val)
{
	_kernel->setKernelArg(_argOffset, sizeof(cl_char), val);
	_argOffset += sizeof(cl_char);
	return *this;
}

KernelArgs& KernelArgs::operator << (const unsigned char * val)
{
	_kernel->setKernelArg(_argOffset, sizeof(cl_uchar), val);
	_argOffset += sizeof(cl_uchar);
	return *this;
}

KernelArgs& KernelArgs::operator << (const short * val)

{
	_kernel->setKernelArg(_argOffset, sizeof(cl_short), val);
	_argOffset += sizeof(cl_short);
	return *this;
}

KernelArgs& KernelArgs::operator << (const unsigned short * val)
{
	_kernel->setKernelArg(_argOffset, sizeof(cl_ushort), val);
	_argOffset += sizeof(cl_ushort);
	return *this;
}

KernelArgs& KernelArgs::operator << (const int * val)
{
	_kernel->setKernelArg(_argOffset, sizeof(cl_int), val);
	_argOffset += sizeof(cl_int);
	return *this;
}

KernelArgs& KernelArgs::operator << (const unsigned int * val)
{
	_kernel->setKernelArg(_argOffset, sizeof(cl_uint), val);
	_argOffset += sizeof(cl_uint);
	return *this;
}

KernelArgs& KernelArgs::operator << (const long * val)
{
	_kernel->setKernelArg(_argOffset, sizeof(cl_long), val);
	_argOffset += sizeof(cl_long);
	return *this;
}

KernelArgs& KernelArgs::operator << (const unsigned long * val)
{
	_kernel->setKernelArg(_argOffset, sizeof(cl_ulong), val);
	_argOffset += sizeof(cl_ulong);
	return *this;
}

KernelArgs& KernelArgs::operator << (const float * val)
{
	_kernel->setKernelArg(_argOffset, sizeof(cl_float), val);
	_argOffset += sizeof(cl_float);
	return *this;
}

KernelArgs& KernelArgs::operator << (const double * val)
{
	_kernel->setKernelArg(_argOffset, sizeof(cl_double), val);
	_argOffset += sizeof(cl_double);
	return *this;
}

KernelArgs& KernelArgs::operator << (const hsa::RegisterMemory* memory)
{
	_kernel->setKernelArg(_argOffset, sizeof(cl_mem), memory->ptr());
	_argOffset += sizeof(cl_mem);
	return *this;
}
