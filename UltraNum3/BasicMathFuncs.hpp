/*

(C) Matthew Swanson

This file is part of UltraNum2.

UltraNum2 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

UltraNum2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with UltraNum2.  If not, see <http://www.gnu.org/licenses/>.

*/
#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>

#include "Helpers.hpp"
#include "Type.hpp"
#include "BasicBits.hpp"

namespace cg
{

/**Determine if an array is zero.
\param arr The array.
\param s The size of the array.
\return True if the array is zero.*/
template<typename T>
bool IsZero(const T* arr, std::size_t s)
{
	return std::count(arr, arr + s, 0) == s;
}
/**Determine if an array is one.
\param arr The array.
\param s The size of the array.
\return True if the array is zero.*/
template<typename T>
bool IsOne(const T* arr, std::size_t s)
{
	return (*arr == 1) && std::count(arr + 1, arr + s, 0) == s;
}

/**Compare two objects.  This function assumes that there are no leading zeros
that do not effect the value of the function.
\param arr1 The first array.
\param s1 the max size of arr1.
\param arr2 The second array.
\param s2 The max size of r2.
\return -1 if arr1 < arr2, 0 if arr1 == arr2, 1 if arr1 > arr2.*/
template<typename T>
int CompareArray(const T* arr1, std::size_t s1, const T* arr2, std::size_t s2)
{
	if (s1 < s2)
		/*arr1 < arr2*/
		return -1;
	if (s1 > s2)
		/*arr1 > arr2*/
		return 1;

	auto beg1 = arr1 + s1 - 1;
	auto beg2 = arr2 + s2 - 1;
	auto end = arr1 - 1;
	for (; beg1 != end; --beg1, --beg2)
	{
		if (*beg1 < *beg2)
			return -1;
		if (*beg1 > *beg2)
			return 1;
	}
	return 0;
}


/**Array adding function.  The carry will propagate over
adjacent pointers up to the amount in s1.
\param arr1 The first array.
\param s1 the max size of arr1.
\param arr2 The second array.
\param s2 The max size of r2.
\return false always.*/
template<typename T>
bool AddArray(T* arr1, std::size_t s1, const T* arr2, std::size_t s2)
{
	auto & l = *arr1;
	auto & r = *arr2;
	l += r;
	if (l < r && s1 != 1)
	{
		T t(1);
		AddArray(arr1 + 1, s1 - 1, &t, 1);
	}
	if (s2 != 1 && s1 != 1)
		AddArray(arr1 + 1, s1 - 1, arr2 + 1, s2 - 1);
	return false;
}
/**Add a single num to the array.
\param arr The array.
\param s The size of the array.
\param n A number to add to the array.*/
template<typename T>
void AddArray(T* arr, std::size_t s, const T& n)
{
	AddArray(arr, s, &n, 1);
}
/**The sub function.  The borrow will propagate over
adjacent pointers up to the amount in s1.
\param arr1 The first array.
\param s1 the max size of arr1.
\param arr2 The second array.
\param s2 The max size of r2.
\return True if the result went below zero. False if its still > 0.*/
template<typename T>
bool SubArray(T* arr1, std::size_t s1, const T* arr2, std::size_t s2)
{
	auto & l = *arr1;
	bool doCarry = *arr2 > l;
	auto r = cg::TwoComp(*arr2);
	if (r != 0)
	{
		l += r;
		if (doCarry)
		{
			T t(1);
			/*supress the carry with size 1 for arr1.  We ignore the carry when
			`subbing` the `1` while in twos comp form.*/
			SubArray(arr1 + 1, 1, &t, 1);
		}
	}
	if (s2 != 1 && s1 != 1)
		SubArray(arr1 + 1, s1 - 1, arr2 + 1, s2 - 1);

	return false;
}

/**Sub a single num from the array.
\param arr The array.
\param s The size of the array.
\param n A number to sub from the array.*/
template<typename T>
void SubArray(T* arr, std::size_t s, const T& n)
{
	SubArray(arr, s, &n, 1);
}
/**The mult function.  The borrow will propagate over
adjacent pointers up to the amount in s1.  Should be called with T = a type
that is half the size of the actual type.
\param arr1 The first array.
\param s1 the max size of arr1.
\param arr2 The second array.
\param s2 The max size of r2.
\return false always.*/
template<typename T>
bool MulArray(T* arr1, std::size_t s1, const T* arr2, std::size_t s2)
{
	static_assert(sizeof(T) > 1, "T must be at least 2 bytes long.");
	using DT = typename cg::DemoteType<T>::Type;
	/*The arrays are casted to half the size type, so double the amount of
	units.*/
	s1 += s1;
	s2 += s2;
	DT* tArr = new DT[s1]();

	for (std::size_t i = 0; i < s1; ++i)
	{
		for (std::size_t j = 0; j < s2; ++j)
		{
			T t = T(((DT*)arr1)[i]) * ((DT*)arr2)[j];
			AddArray(tArr + i + j, s1 - i - j, (DT*)&t, 2);
		}
	}

	std::memmove(arr1, tArr, s1 * sizeof(DT));
	return false;
}
/**Iteger power function.
\param num A reference to the number to apply to the power.
\param exp The exponent to apply.
\return A reference to num*/
template<typename T>
inline T& PowInPlace(T& num, const T& exp)
{
	if (exp == 0)
	{
		num = 1;
		return num;
	}
	T base = num;
	num = 1;
	while (exp)
	{
		if (exp & 1)
			num *= base;
		exp >>= 1;
		base *= base;
	}
	return num;

}
/**The basic division function.  This function assumes there are no MSB zeros.
\param arr1 The first dividend array.
\param s1 The size of the first array.
\param arr2 The second divisor array.
\param s2 The size of the second array.
\param arr3 The third array that will hold the modulo of the operation. Iff its
nullptr (or 0) it will be ignored.  If its not false, it must be the same size
as s1.
\return false always.*/
template<typename T>
bool DivArray_Shift(T* arr1, std::size_t s1, const T* arr2, std::size_t s2,
	T* arr3)
{
	static_assert(sizeof(T) > 1, "T must be at least 2 bytes long.");
	if (s1 < s2)
	{
		if (arr3 != nullptr)
		{
			std::memset(arr3, 0, s1 * sizeof(T));
			std::memmove(arr3, arr1, s1 * sizeof(T));
		}
		std::memset(arr1, 0, s1 * sizeof(T));
		return false;
	}
	if (IsZero(arr2, s2))
		throw std::invalid_argument("Divisor is zero.");
	T* tBuf = new T[s1]();
	T* counter = new T[s1]();
	std::size_t shifted = 0;
	std::memmove(tBuf, arr2, s2 * sizeof(T));
	std::size_t msb_tBuf = MSBNumber(tBuf, s1);
	std::size_t msb_arr1 = MSBNumber(arr1, s1);
	std::size_t shfAmt = (msb_arr1 - 2) - msb_tBuf;
	ShiftSigB(tBuf, s1, shfAmt);
	int c = CompareArray(arr1, s1, tBuf, s1);
	if (c == 0)
	{
		std::memset(arr1, 0, s1 * sizeof(T));
		std::memset(arr3, 0, s1 * sizeof(T));
		*arr1 = 1;
		return false;
	}
	bool go = c == 1;
	while (go && !IsZero(tBuf, s1))
	{
		SubArray(arr1, s1, tBuf, s1);
		AddArray<T>(counter, s1, 1);
		//if (!IsZero(arr1, s1))
		//{
			go = CompareArray(arr1, s1, tBuf, s1) == 1;
		//}
		//else
		//	go = false;
		while (!go)
		{
			if (shfAmt == 0)
			{
				go = false;
				break;
			}
			ShiftInsigB(tBuf, s1, 1);
			--shfAmt;
			ShiftSigB(counter, s1, 1);
			int c = CompareArray(arr1, s1, tBuf, s1);
			go = c == 1 || c == 0;
		}
	}
	if (arr3 != nullptr)
	{
		std::memset(arr3, 0, s1 * sizeof(T));
		std::memmove(arr3, arr1, s1 * sizeof(T));
	}
	std::memmove(arr1, counter, s1 * sizeof(T));
	return false;
}

}