#ifndef _HSA_KERNEL_H_
#define _HSA_KERNEL_H_

#include <hsa/Base.h>

namespace hsa
{
	class Context;

	class Kernel
	{
		private:
			Context *_context;
			int _status;
			void *_argPtr;
			void *_kernel_arg_buffer;
			hsa_ext_code_descriptor_t *_hsaCodeDescriptor;
			
		public:
			Kernel(Context *context, hsa_ext_code_descriptor_t *hsaCodeDescriptor);
			~Kernel();
			Kernel& operator << (void *val);
			//Kernel& operator << (hsa::RegisterMemory& memory);
			int status() const;
	};
} // namespace hsa

#endif
