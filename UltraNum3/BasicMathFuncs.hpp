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

#include "Helpers.hpp"
#include "Type.hpp"

namespace cg
{

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
/**Shift the array toward the significant side.  This function shifts enire
units of T.
\param arr1 The array.
\param s1 The size of the array.
\param amt The amount to shift.*/
template<typename T>
void ShiftSig(T* arr1, std::size_t s1, std::size_t amt)
{
	if (amt >= s1)
	{
		std::memset(arr1, 0, s1 * sizeof(T));
		return;
	}
	std::memmove(arr1 + amt, arr1, (s1 - amt) * sizeof(T));
	std::memset(arr1, 0, amt * sizeof(T));
	return;
}
/**Shift the array toward the significant side.  This function shifts bits.
\param arr1 The array.
\param s1 The size of the array.
\param amt The amount to shift (in bits).*/
template<typename T>
void ShiftSigB(T* arr1, std::size_t s1, std::size_t amt)
{
	const static std::size_t TBits = (sizeof(T) * 8);
	std::size_t bytes = (amt / 8);
	std::size_t leftOver = amt % 8;
	if (bytes > 0)
		ShiftSig<uint8_t>((uint8_t*)arr1, s1 * sizeof(T), bytes);
	T carry = 0;
	auto beg = arr1;
	auto end = arr1 + s1;
	for (; beg != end; ++beg)
	{
		T nC = *beg >> (TBits - leftOver);
		*beg <<= leftOver;
		*beg |= carry;
		carry = nC;
	}
	return;
}
/**Shift the array toward the significant side.  This function shifts enire
units of T.
\param arr1 The array.
\param s1 The size of the array.
\param amt The amount to shift.*/
template<typename T>
void ShiftInsig(T* arr1, std::size_t s1, std::size_t amt)
{
	if (amt >= s1)
	{
		std::memset(arr1, 0, s1 * sizeof(T));
		return;
	}
	std::memmove(arr1, arr1 + amt, (s1 - amt) * sizeof(T));
	std::memset(arr1 + (s1 - amt), 0, amt * sizeof(T));
	return;
}
/**Shift the array toward the significant side.  This function shifts bits.
\param arr1 The array.
\param s1 The size of the array.
\param amt The amount to shift.*/
template<typename T>
void ShiftInsigB(T* arr1, std::size_t s1, std::size_t amt)
{
	const static std::size_t TBits = (sizeof(T) * 8);
	std::size_t bytes = (amt / 8);
	std::size_t leftOver = amt % 8;
	if (bytes > 0)
		ShiftInsig<uint8_t>((uint8_t*)arr1, s1 * sizeof(T), bytes);
	T carry = 0;
	auto beg = arr1 + s1 -1;
	auto end = arr1 -1;
	for (; beg != end; --beg)
	{
		T nC = *beg << (TBits - leftOver);
		*beg >>= leftOver;
		*beg |= carry;
		carry = nC;
	}
	return;
}

/**The basic division function.
\param arr1 The first dividend array.
\param s1 The size of the first array.
\param arr2 The second divisor array.
\param s2 The size of the second array.
\return false always.*/
template<typename T>
bool DivArray(T* arr1, std::size_t s1, const T* arr2, std::size_t s2)
{
	static_assert(sizeof(T) > 1, "T must be at least 2 bytes long.");
	T* tBuf = new T[s1];
	std::memmove(tBuf, arr2, s2 * sizeof(T));


	return false;
}

}