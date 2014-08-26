#ifndef _REGISTERMEMORY_H_
#define _REGISTERMEMORY_H_

#include <hsa/Base.h>

namespace hsa
{
	class KernelArgs;

	class RegisterMemory
	{
		friend class KernelArgs;

		private:
			size_t _size;
			void* _address;

		protected:
			void *ptr() const;

		public:
			RegisterMemory(size_t size);
			~RegisterMemory();

			template<typename _Tp> _Tp* getAddress(int offset=0)
			{
				_Tp *ptr = (_Tp *)_address;
				return ptr+offset;
			}
	};
} // namespace hsa

#endif
