#include <hsa/RegisterMemory.h>
#include <hsa/Exception.h>
#include <hsa/Kernel.h>
#include <cstdlib>

using namespace hsa;

RegisterMemory::RegisterMemory(size_t size) : _size(size), needToFree(true)
{
	if(size==0)
		THROW(RegisterMemory size cannot be zero);

	_address = malloc(size);

    hsa_status_t err=hsa_memory_register(_address, _size);
    CHECK_HSA(Registering argument memory for input parameter, err);
}

RegisterMemory::RegisterMemory(size_t size, void *memory) : _size(size), _address(memory), needToFree(false)
{
	if(size==0)
		THROW(RegisterMemory size cannot be zero);
	
    hsa_status_t err=hsa_memory_register(_address, _size);
    CHECK_HSA(Registering argument memory for input parameter, err);
}

RegisterMemory::~RegisterMemory()
{
    hsa_status_t err=hsa_memory_deregister(_address, _size);
    CHECK_HSA(De-Registering argument memory for input parameter, err);

    if(needToFree)	free(_address);
}

const void *RegisterMemory::ptr() const
{
	return &_address;
}
