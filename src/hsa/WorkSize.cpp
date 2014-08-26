#include <hsa/WorkSize.h>
#include <vector>
#include <cmath>
#include <sstream>

using namespace hsa;

const WorkSize WorkSize::Unset((void*)nullptr);

size_t WorkSize::SumSize() const
{
	size_t sum(1);
	sum *= (d >= 1) ? s[0] : 1;
	sum *= (d >= 2) ? s[1] : 1;
	sum *= (d >= 3) ? s[2] : 1;
	return sum;
}

bool WorkSize::IsMultipleOf(const WorkSize &w) const
{
	size_t sum(0);
	sum += (d >= 1) ? (s[0] % w.s[0]) : 0;
	sum += (d >= 2) ? (s[1] % w.s[1]) : 0;
	sum += (d >= 3) ? (s[2] % w.s[2]) : 0;
	return 0 == sum;
}

void WorkSize::RestrictWith(const WorkSize &w)
{
	if(d >= 1 && s[0] > w.s[0]) s[0] = w.s[0];
	if(d >= 2 && s[1] > w.s[1]) s[1] = w.s[1];
	if(d >= 3 && s[2] > w.s[2]) s[2] = w.s[2];
}

bool operator == (const WorkSize &a, const WorkSize &b)
{
	if (a.Dimensions() != b.Dimensions()) return false;

	bool equal(true);
	for (unsigned int d(0); equal && d < a.Dimensions(); ++d)
	{
		equal &= (a.Sizes()[d] == b.Sizes()[d]);
	}
	return equal;
}

bool operator != (const WorkSize &a, const WorkSize &b)
{
	if (a.Dimensions() != b.Dimensions()) return true;

	for (unsigned int d(0); d < a.Dimensions(); ++d)
	{
		if (a.Sizes()[d] != b.Sizes()[d])
		{
			return true;
		}
	}
	return false;
}

WorkSize operator / (const WorkSize &a, const WorkSize &b)
{
	return WorkSize
	(
		a.Dimensions() < b.Dimensions() ? a.Dimensions() : b.Dimensions(),
		b.Size0() ? a.Size0() / b.Size0() : 0,
		b.Size1() ? a.Size1() / b.Size1() : 0,
		b.Size2() ? a.Size2() / b.Size2() : 0
	);
}

WorkSize operator % (const WorkSize &a, const WorkSize &b)
{
	return WorkSize
	(
		a.Dimensions() < b.Dimensions() ? a.Dimensions() : b.Dimensions(),
		b.Size0() ? a.Size0() % b.Size0() : 0,
		b.Size1() ? a.Size1() % b.Size1() : 0,
		b.Size2() ? a.Size2() % b.Size2() : 0
	);
}

WorkSize operator + (const WorkSize &a, const WorkSize &b)
{
	return WorkSize
	(
		a.Dimensions() < b.Dimensions() ? a.Dimensions() : b.Dimensions(),
		a.Size0() + b.Size0(),
		a.Size1() + b.Size1(),
		a.Size2() + b.Size2()
	);
}

WorkSize operator - (const WorkSize &a, const WorkSize &b)
{
	return WorkSize
	(
		a.Dimensions() < b.Dimensions() ? a.Dimensions() : b.Dimensions(),
		a.Size0() - b.Size0(),
		a.Size1() - b.Size1(),
		a.Size2() - b.Size2()
	);
}

size_t WorkSize::LargestPow2Size(const size_t size)
{
	if (size > 0 && ((size & (size - 1)) != 0))
	{
		size_t result = 1;
		while (result*2 <= size)
			result *= 2;
		return result;
	}
	return size;
}

size_t WorkSize::DivRoundUp(const size_t dividend, const size_t divisor)
{
	// dividend must be > 0
	return ((dividend - 1)/divisor + 1);
}

WorkSize &WorkSize::operator=(const WorkSize &_workSize)
{
	d = _workSize.d;
	s[0] = _workSize.Size0();
	s[1] = _workSize.Size1();
	s[2] = _workSize.Size2();
	return *this;
}

std::string WorkSize::ToString() const
{
	std::stringstream ss;
	ss << '{' << s[0];
	unsigned int i(1);
	while(i < d)
	{
		ss << ", " << s[i++];
	}
	ss << '}' << std::flush;
	return ss.str();
}
