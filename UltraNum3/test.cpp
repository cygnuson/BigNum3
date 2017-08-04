
#include <utility>
#include <random>
#include <ctime>
#include <cassert>

#include "BigNum.hpp"
#include "List.hpp"
#include "Timer.hpp"

uint64_t RandomU64();
uint64_t RandomU64_2();
bool TestBigNumAdd(std::size_t amt);
bool TestBigLShift(std::size_t amt);
bool TestBigRShift(std::size_t amt);
bool TestBigNumCompare(std::size_t amt);
bool TestBigNumSub(std::size_t amt);
bool TestBigNumMul(std::size_t amt);

int main()
{

	uint64_t n1 = 7116534513545867317;
	auto a = cg::BigNum<uint16_t, 4>();
	a.PushArray(cg::AsArray<uint16_t>(&n1), 4);

	auto bAns = *((uint64_t*)a.Begin());

	auto a2 = cg::BigNum<uint16_t, 4>(0,1,0,0);
	

	auto beg = a2.Begin();
	auto end = a2.End();
	for (; beg != end; ++beg)
		;// std::cout << *beg << ",";

	TestBigLShift(1000000);
	TestBigRShift(1000000);
	TestBigNumSub(1000000);
	TestBigNumCompare(1000000);
	TestBigNumAdd(1000000);
	TestBigNumMul(1000000);

	int stop = 0;
	return stop;
}

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
uint64_t RandomU64_2()
{
	uint64_t n = 0;
	auto a = cg::AsArray<uint16_t>(n);
	a[0] = std::rand() % 65535;
	a[1] = std::rand() % 65535;
	a[2] = std::rand() % 65535;
	a[3] = std::rand() % 65535;
	return n;
}

bool TestBigNumAdd(std::size_t amt)
{
	std::srand((unsigned int)std::time(0));
	double time = 0.0;
	for (std::size_t i = 0; i < amt; ++i)
	{
		uint64_t n1 = RandomU64();
		uint64_t n2 = RandomU64();
		auto a = cg::BigNum<uint16_t, 4>();
		a.PushArray(cg::AsArray<uint16_t>(n1), 4);
		auto b = cg::BigNum<uint16_t, 4>();
		b.PushArray(cg::AsArray<uint16_t>(n2), 4);

		uint64_t answer = n1 + n2;
		auto funcLambda = [&]()
		{
			a += b;
		};
		time += cg::Timer::TimedCall(funcLambda).count();

		auto bAns = *((uint64_t*)a.Begin());

		assert(answer == bAns);
	}

	std::cout << " Add: " << time / amt << std::endl;

	return false;
}
bool TestBigNumCompare(std::size_t amt)
{
	std::srand((unsigned int)std::time(0));
	double time = 0.0;
	cg::Timer dt;
	for (std::size_t i = 0; i < amt; ++i)
	{
		uint64_t n1 = RandomU64_2();
		uint64_t n2 = RandomU64_2();
		auto a = cg::BigNum<uint16_t, 4>();
		a.PushArray(cg::AsArray<uint16_t>(n1), 4);
		auto b = cg::BigNum<uint16_t, 4>();
		b.PushArray(cg::AsArray<uint16_t>(n2), 4);

		bool lt = n1 < n2;
		bool ne = n1 != n2;
		dt.Restart();
		bool bnlt = a < b;
		bool bnne = a != b;
		time += dt.Restart().count();

		assert(bnlt == lt && bnne == ne);
	}
	time /= amt;
	std::cout << "Comp: " << time / amt << std::endl;

	return false;
}
bool TestBigNumSub(std::size_t amt)
{
	std::srand((unsigned int)std::time(0));
	double time = 0.0;
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
		a.PushArray(cg::AsArray<uint16_t>(n1), 4);
		auto b = cg::BigNum<uint16_t, 4>();
		b.PushArray(cg::AsArray<uint16_t>(n2), 4);
		uint64_t answer = n1 - n2;
		auto funcLambda = [&]()
		{
			a -= b;
		};
		time += cg::Timer::TimedCall(funcLambda).count();

		auto bAns = *((uint64_t*)a.Begin());

		assert(answer == bAns);
	}
	std::cout << " Sub: " << time / amt << std::endl;

	return false;
}
bool TestBigNumMul(std::size_t amt)
{
	std::srand((unsigned int)std::time(0));
	double time = 0.0;
	for (std::size_t i = 0; i < amt; ++i)
	{
		uint32_t n1 = (uint32_t) RandomU64();
		uint32_t n2 = (uint32_t) RandomU64();
		auto a = cg::BigNum<uint16_t, 4>();
		a.PushArray(cg::AsArray<uint16_t>(n1), 2);
		auto b = cg::BigNum<uint16_t, 2>();
		b.PushArray(cg::AsArray<uint16_t>(n2), 2);
		uint64_t answer = uint64_t(n1) * n2;
		auto funcLambda = [&]()
		{
			a *= b;
		};
		time += cg::Timer::TimedCall(funcLambda).count();

		auto bAns = *((uint64_t*)a.Begin());

		assert(answer == bAns);
	}
	std::cout << " Mul: " << time / amt << std::endl;

	return false;
}
bool TestBigLShift(std::size_t amt)
{
	std::srand((unsigned int)std::time(0));
	double time = 0.0;
	for (std::size_t i = 0; i < amt; ++i)
	{
		uint64_t n1 = RandomU64_2();
		uint64_t originN1 = n1;
		auto a = cg::BigNum<uint16_t, 4>();
		a.PushArray(cg::AsArray<uint16_t>(n1), 4);
		auto originA = cg::BigNum<uint16_t, 4>();
		originA.PushArray(cg::AsArray<uint16_t>(n1), 4);
		int shiftAmt = rand() % 64;
		n1 <<= shiftAmt;
		auto funcLambda = [&]()
		{
			a <<= shiftAmt;
		};
		time += cg::Timer::TimedCall(funcLambda).count();

		auto bAns = *((uint64_t*)a.Begin());

		assert(n1 == bAns);
	}

	std::cout << "LShf: " << time / amt << std::endl;

	return false;
}
bool TestBigRShift(std::size_t amt)
{
	std::srand((unsigned int)std::time(0));
	double time = 0.0;
	for (std::size_t i = 0; i < amt; ++i)
	{
		uint64_t n1 = RandomU64_2();
		uint64_t originN1 = n1;
		auto a = cg::BigNum<uint16_t, 4>();
		a.PushArray(cg::AsArray<uint16_t>(n1), 4);
		auto originA = cg::BigNum<uint16_t, 4>();
		originA.PushArray(cg::AsArray<uint16_t>(n1), 4);
		int shiftAmt = rand() % 64;
		n1 >>= shiftAmt;
		auto funcLambda = [&]()
		{
			a >>= shiftAmt;
		};
		time += cg::Timer::TimedCall(funcLambda).count();

		auto bAns = *((uint64_t*)a.Begin());

		assert(n1 == bAns);
	}

	std::cout << "RShf: " << time / amt << std::endl;

	return false;
}
