#ifndef _HSA_KERNEL_H_
#define _HSA_KERNEL_H_

#include <hsa/Base.h>

namespace hsa
{
	class Context;
	class RegisterMemory;
	class CommandQueue;

	class Kernel
	{
		friend class CommandQueue;

		private:
			Context *_context;
			void *_kernel_arg_buffer;
			hsa_ext_code_descriptor_t *_hsaCodeDescriptor;
		protected:
			uint64_t getKernArgAddress() const;
			uint64_t getExtCodeHandle() const;
		public:
			Kernel(Context *context, hsa_ext_code_descriptor_t *hsaCodeDescriptor);
			~Kernel();
			void operator()(unsigned int argumentIndex, void *val);
	};

	class KernelArgs
	{
		private:
			int _argIndex;
			hsa::Kernel *_kernel;

		public:
			KernelArgs(hsa::Kernel *kernel);

			KernelArgs& operator << (void *val);
			KernelArgs& operator << (hsa::RegisterMemory* memory);
	};
} // namespace hsa

#endif
