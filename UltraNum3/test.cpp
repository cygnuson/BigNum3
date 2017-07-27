
#include <utility>

//#include "Rational.hpp"
//#include "Int.hpp"
//#include "List.hpp"

#include "IntImpl.hpp"


int main()
{
	cg::IntImpl<uint16_t, 0> a = { 3,4,5,6 };
	auto B = a.Lo();
	auto X = B->Lo();
	X->Get(0) = 111;

	int stop = 0;
	return stop;
}