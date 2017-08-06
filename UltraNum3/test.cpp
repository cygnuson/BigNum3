
#include <utility>
#include <random>
#include <ctime>
#include <cassert>
#include <iostream>

#include "BigNum.hpp"
#include "List.hpp"
#include "Timer.hpp"

uint64_t RandomU64();
uint64_t RandomU64_2();
bool TestBigNumAdd(std::size_t amt);
bool TestBigNumDiv(std::size_t amt);
bool TestBigNumMod(std::size_t amt);
bool TestBigLShift(std::size_t amt);
bool TestBigRShift(std::size_t amt);
bool TestBigNumCompare(std::size_t amt);
bool TestBigNumSub(std::size_t amt);
bool TestBigNumMul(std::size_t amt);

int main()
{
	/*uint64_t n1 = 5356324927;
	uint64_t n2 = 128216;
	auto a = cg::BigNum<uint16_t, 4>();
	a.PushArray(cg::AsArray<uint16_t>(n1), 4);
	auto b = cg::BigNum<uint16_t, 4>();
	b.PushArray(cg::AsArray<uint16_t>(n2), 4);

	uint64_t answer = n1 % n2;
	auto funcLambda = [&]()
	{
		a %= b;
	};
	funcLambda();

	auto bAns = *((uint64_t*)a.Begin());

	assert(answer == bAns);*/



	TestBigLShift		(100000);
	TestBigRShift		(100000);
	TestBigNumSub		(100000);
	TestBigNumDiv		(100000);
	TestBigNumCompare	(100000);
	TestBigNumAdd		(100000);
	TestBigNumMul		(100000);
	TestBigNumMod		(100000);

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
bool TestBigNumDiv(std::size_t amt)
{
	std::srand((unsigned int)std::time(0));
	double time = 0.0;
	for (std::size_t i = 0; i < amt; ++i)
	{
		uint64_t n1 = RandomU64();
		uint64_t n2 = RandomU64() % 200000;
		if (n1 < n2)
		{
			auto x = n1;
			n1 = n2;
			n2 = x;
		}
		if (n2 == 0)
			n2 = 1;
		auto a = cg::BigNum<uint16_t, 4>();
		a.PushArray(cg::AsArray<uint16_t>(n1), 4);
		auto b = cg::BigNum<uint16_t, 4>();
		b.PushArray(cg::AsArray<uint16_t>(n2), 4);
		uint64_t answer = n1 / n2;
		auto funcLambda = [&]()
		{
			a /= b;
		};
		time += cg::Timer::TimedCall(funcLambda).count();

		auto bAns = *((uint64_t*)a.Begin());

		assert(answer == bAns);
	}
	std::cout << " Div: " << time / amt << std::endl;

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

bool TestBigNumMod(std::size_t amt)
{
	std::srand((unsigned int)std::time(0));
	double time = 0.0;
	for (std::size_t i = 0; i < amt; ++i)
	{
		uint64_t n1 = RandomU64();
		uint64_t n2 = RandomU64() % 200000;
		if (n1 < n2)
		{
			auto x = n1;
			n1 = n2;
			n2 = x;
		}
		if (n2 == 0)
			n2 = 1;
		auto a = cg::BigNum<uint16_t, 4>();
		a.PushArray(cg::AsArray<uint16_t>(n1), 4);
		auto b = cg::BigNum<uint16_t, 4>();
		b.PushArray(cg::AsArray<uint16_t>(n2), 4);
		uint64_t answer = n1 % n2;
		auto funcLambda = [&]()
		{
			a %= b;
		};
		time += cg::Timer::TimedCall(funcLambda).count();

		auto bAns = *((uint64_t*)a.Begin());

		assert(answer == bAns);
	}
	std::cout << " Mod: " << time / amt << std::endl;

	return false;
}