
#include <utility>

#include "Num.hpp"
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

int main(int argc, char ** argv)
{
	auto r = cg::Rational::Make(0.444329, 9);
	auto a = r.MInverse();
	r.AutoSimp(true);

	auto X = r.Eval<double>();

	int stop = 0;
	return stop;
}