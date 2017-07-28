
#include <utility>

#include "Num.hpp"

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

template<const bool e>
bool r()
{
	return e;
}

int main(int argc, char ** argv)
{


	int stop = 0;
	return stop;
}