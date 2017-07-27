
#include <utility>

#include "Num.hpp"


int main()
{
	uint16_t n = 4;
	cg::Num<uint16_t&> a(n);

	auto X = a.GetReference();
	X.Get() = 9999;
	auto Y = X.HardCopy();
	Y.Get() = 333;
	X.Get() = 44;

	int stop = 0;
	return stop;
}