
#include <utility>

#include "BigNum.hpp"
#include "List.hpp"

const uint16_t& testfunc(const uint16_t& t)
{
	return t;
}
uint16_t& testfunc(uint16_t& t)
{
	return t;
}

struct testclass {
	uint16_t& Get()
	{
		return n;
	}
	const uint16_t& Get() const
	{
		return n;
	}
	uint16_t n = 5555;
};

template<bool UseStack>
class Points
{
public:
	/* ... Member functions ...*/
private:
	cg::List<int, UseStack ? 100 : 0> m_points;
};

#include "Rational.hpp"

int main()
{
	uint16_t a = 4;
	uint16_t b = 5;


	cg::BigNum<uint16_t&, 2> n(a, b);
	a = 6;
	n.Hi().Get(0) = 99;
	auto beg = n.Begin();
	auto end = n.End();
	for (; beg != end; ++beg)
		std::cout << beg->Get(0) << ", ";

	int stop = 0;
	return stop;
}