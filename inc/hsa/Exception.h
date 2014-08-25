#ifndef _HSA_EXCEPTION_H_
#define _HSA_EXCEPTION_H_

#include <hsa/Base.h>
#include <hsa/elf_utils.h>

#define CHECK_HSA(msg, status) hsa::Exception::Check(#msg, status, __FILE__, __LINE__);
#define THROW(msg) throw new hsa::Exception((msg), __FILE__, __LINE__);

namespace hsa
{
	class Exception
	{
		public:
			const char *status_str;
			Exception(const hsa_status_t &status, const char *_file_, int _line_, const char *message=nullptr);
			Exception(const status_t &status, const char *_file_, int _line_, const char *message=nullptr);
			static void Check(const char* message, const hsa_status_t &status, const char *_file_ = "unknown", int _line_ = 0);
			static void Check(const char* message, const status_t &status, const char *_file_ = "unknown", int _line_ = 0);
	};
}

#endif
