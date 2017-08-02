
#include <utility>
#include <random>
#include <ctime>
#include <cassert>

#include "BigNum.hpp"
#include "List.hpp"

uint64_t RandomU64()
{
	uint64_t n = 0;
	auto a = cg::AsArray<uint16_t>(n);
	a[0] = std::rand() % 65535;
	a[1] = std::rand() % 65535;
	a[2] = std::rand() % 65535;
	a[3] = 0;
	return n;
}

bool TestBigNumAdd(std::size_t amt)
{
	std::srand(std::time(0));
	for (std::size_t i = 0; i < amt; ++i)
	{
		uint64_t n1 = RandomU64();
		uint64_t n2 = RandomU64();
		auto a = cg::BigNum<uint16_t, 4>();
		a.AddArray(cg::AsArray<uint16_t>(n1), 4);
		auto b = cg::BigNum<uint16_t, 4>();
		b.AddArray(cg::AsArray<uint16_t>(n2), 4);

		uint64_t answer = n1 + n2;
		a += b;

		auto bAns = *((uint64_t*)a.Begin());

		assert(answer == bAns);
	}


	return false;
}
bool TestBigNumSub(std::size_t amt)
{
	std::srand(std::time(0));
	for (std::size_t i = 0; i < amt; ++i)
	{
		uint64_t n1 = RandomU64();
		uint64_t n2 = RandomU64();
		if (n1 < n2)
		{
			auto x = n1;
			n1 = n2;
			n2 = x;
		}
		auto a = cg::BigNum<uint16_t, 4>();
		a.AddArray(cg::AsArray<uint16_t>(n1), 4);
		auto b = cg::BigNum<uint16_t, 4>();
		b.AddArray(cg::AsArray<uint16_t>(n2), 4);

		uint64_t answer = n1 - n2;
		a -= b;

		auto bAns = *((uint64_t*)a.Begin());

		assert(answer == bAns);
	}


	return false;
}


int main()
{


	auto a = cg::BigNum<uint16_t, 4>(2, 3, 4, 5);
	auto b = cg::BigNum<uint16_t, 4>(1, 1, 1, 1);
	a -= b;

	auto beg = a.Begin();
	auto end = a.End();
	for (; beg != end; ++beg)
		std::cout << *beg << ", ";


	TestBigNumSub(1000000);

	int stop = 0;
	return stop;
}