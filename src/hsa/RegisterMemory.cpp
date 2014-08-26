#include <hsa/RegisterMemory.h>
#include <hsa/Exception.h>
#include <hsa/Kernel.h>
#include <cstdlib>

using namespace hsa;

RegisterMemory::RegisterMemory(size_t size) : _size(size)
{
	if(size==0)
		THROW(RegisterMemory size cannot be zero);

	_address = malloc(size);

    hsa_status_t err=hsa_memory_register(_address, _size);
    CHECK_HSA(Registering argument memory for input parameter, err);
}

RegisterMemory::~RegisterMemory()
{
	free(_address);

    hsa_status_t err=hsa_memory_deregister(_address, _size);
    CHECK_HSA(De-Registering argument memory for input parameter, err);
}

void *RegisterMemory::ptr() const
{
	return (void *)&_address;
}
