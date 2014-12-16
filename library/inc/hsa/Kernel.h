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
		friend class KernelArgs;

		private:
			Context *_context;
			void *_kernel_arg_buffer;
			hsa_ext_code_descriptor_t *_hsaCodeDescriptor;
		protected:
			uint64_t getKernArgAddress() const;
			uint64_t getExtCodeHandle() const;
			uint32_t getGroupSegmentSize() const;
			uint32_t getPrivateSegmentSize() const;
		public:
			void setKernelArg(unsigned int offset, size_t size, const void *argument);
			Kernel(Context *context, hsa_ext_code_descriptor_t *hsaCodeDescriptor);
			~Kernel();
	};

	enum Alignment 
	{ 
		JMP_ONE=1, JMP_TWO=2, JMP_THREE=3, JMP_FOUR=4, 
		JMP_FIVE=5, JMP_SIX=6, JMP_SEVEN=7 
	};
	
	class KernelArgs
	{
		private:
			unsigned int _argOffset;
			hsa::Kernel *_kernel;

		public:
			KernelArgs(hsa::Kernel *kernel);
			
			//Strange alignment for kernel alignment.
			KernelArgs& operator << (enum Alignment alignment);
			
			//Scalar
			KernelArgs& operator << (const char *val);
			KernelArgs& operator << (const unsigned char *val);
			KernelArgs& operator << (const short *val);
			KernelArgs& operator << (const unsigned short *val);
			KernelArgs& operator << (const int *val);
			KernelArgs& operator << (const unsigned int *val);
			KernelArgs& operator << (const long *val);
			KernelArgs& operator << (const unsigned long *val);
			KernelArgs& operator << (const float *val);
			KernelArgs& operator << (const double *val);
			
			//Memory
			KernelArgs& operator << (const hsa::RegisterMemory* memory);	//global memory
	};
} // namespace hsa

#endif
