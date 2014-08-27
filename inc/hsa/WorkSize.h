#ifndef _HSA_WORKSIZE_H_
#define _HSA_WORKSIZE_H_

#include <iostream>
#include <string>
#include <hsa/Base.h>

namespace hsa
{
	class WorkSize
	{
		private:
			size_t s[3];
			unsigned int d;
			WorkSize(void *) : d(0) { s[0] = 0; s[1] = 0; s[2] = 0; }

		public:
			const size_t *Sizes() const { return (d > 0) ? s : nullptr; }
			size_t Size0() const { return s[0]; }
			size_t Size1() const { return s[1]; }
			size_t Size2() const { return s[2]; }
			unsigned int Dimensions() const { return d; }

			size_t SumSize() const;
			bool IsMultipleOf(const WorkSize &w) const;
			void RestrictWith(const WorkSize &w);

			friend bool operator == (const WorkSize &a, const WorkSize &b);
			friend bool operator != (const WorkSize &a, const WorkSize &b);
			friend WorkSize operator / (const WorkSize &a, const WorkSize &b);
			friend WorkSize operator % (const WorkSize &a, const WorkSize &b);
			friend WorkSize operator + (const WorkSize &a, const WorkSize &b);
			friend WorkSize operator - (const WorkSize &a, const WorkSize &b);

			static const WorkSize Unset;

			static size_t LargestPow2Size(const size_t size);
			static size_t DivRoundUp(const size_t dividend, const size_t divisor);

			WorkSize(const size_t _s0) : d(1) { s[0] = _s0, s[1] = 1, s[2] = 1; }
			WorkSize(const size_t _s0, const size_t _s1) : d(2) { s[0] = _s0, s[1] = _s1, s[2] = 1; }
			WorkSize(const size_t _s0, const size_t _s1, const size_t _s2) : d(3) { s[0] = _s0, s[1] = _s1, s[2] = _s2; }
			WorkSize(const unsigned int _d, const size_t _s0, const size_t _s1, const size_t _s2) : d(_d) { s[0] = _s0, s[1] = _s1, s[2] = _s2; }
			WorkSize(const WorkSize &_workSize) : d(_workSize.Dimensions()) { s[0] = _workSize.Size0(); s[1] = _workSize.Size1(); s[2] = _workSize.Size2(); }

			WorkSize &operator = (const WorkSize &_workSize);

			std::string ToString() const;
	};
}

#endif
