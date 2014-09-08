#ifndef _HSA_PROGRAM_H_
#define _HSA_PROGRAM_H_

#include <hsa/Base.h>
#include <vector>

namespace hsa
{
	class Context;
	
	class Kernel;

	class Program
	{
		private:
			Context *_context;
			std::vector<Kernel *> kernels;

			hsa_ext_brig_module_t* _brigModule;
			hsa_ext_program_handle_t _hsaProgram;
			hsa_ext_brig_module_handle_t _moduleHandle;
		public:
			Program(Context *context, const char *fileName);
			~Program();
			Kernel *operator[] (const char *kernelName);
	};
} // namespace hsa

#endif
