
#include <utility>

#include "BigNum.hpp"
#include "List.hpp"


int main()
{
	auto a = cg::BigNum<const uint16_t&, 6>(1, 1, 1, 1, 1, 1);


	auto X = cg::MakeBigNum(a);
	auto Y = cg::MakeBigNum(X);
	auto Z = a.Copy();

	auto beg = a.Begin();
	auto end = a.End();
	for (; beg != end; ++beg)
		std::cout << beg->Get() << ", ";


	int stop = 0;
	return stop;
}