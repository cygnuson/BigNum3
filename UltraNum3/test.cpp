
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
	uint16_t c = 6;
	uint16_t d = 7;


	cg::BigNum<uint16_t&, 4> n(a, b, c, d);
	a = 11;
	b = 55;
	auto X = cg::Hi(n);
	X.Get(0).Get(0) = 88;
	X.Get(1).Get(0) = 33;
	auto beg = n.Begin();
	auto end = n.End();
	for (; beg != end; ++beg)
		std::cout << beg->Get(0) << ", ";

	uint16_t someInt = 257;
	cg::Num<uint16_t&> in(someInt);
	auto G = cg::Hi(in);
	G.Get(0) = 0;

	int stop = 0;
	return stop;
}