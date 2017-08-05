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
#include "ArrayView.hpp"

namespace cg 
{

/**Count the amount of consecutive zeros. If `begin` comes after `end`, `begin`
will be incrimented with '--' instead of '++'.
\param begin The first poitner.
\param end The one-past-the-end pointer.
\return The amount of zeros.*/
template<typename T>
std::size_t CountZeros(const T* begin, const T* end)
{
	std::size_t amt = 0;
	if (end - begin < 0)
	{
		for (; begin != end; --begin)
		{
			if (*begin == 0)
				++amt;
			else
				break;
		}
	}
	else
	{
		for (; begin != end; ++begin)
		{
			if (*begin == 0)
				++amt;
			else
				break;
		}
	}
	return amt;
}

/**Shift the array toward the significant side.  This function shifts enire
units of T.
\param arr1 The array.
\param s1 The size of the array.
\param amt The amount to shift.*/
template<typename T>
void ShiftSig(T* arr1, std::size_t s1, std::size_t amt)
{
	if (amt == 0)
		return;
	if (amt >= s1)
	{
		std::memset(arr1, 0, s1 * sizeof(T));
		return;
	}
	std::memmove(arr1 + amt, arr1, (s1 - amt) * sizeof(T));
	std::memset(arr1, 0, amt * sizeof(T));
	return;
}
/**Shift the array toward the significant side.  This function shifts enire
units of T.
\param arr1 The array.
\param amt The amount to shift.*/
template<typename T>
void ShiftSig(cg::ArrayView<T>& arr1, std::size_t amt)
{
	ShiftSig(arr1.Begin(), arr1.Size(), amt);
}
/**Shift the array toward the significant side.  This function shifts bits.
\param arr1 The array.
\param s1 The size of the array.
\param amt The amount to shift (in bits).*/
template<typename T>
void ShiftSigB(T* arr1, std::size_t s1, std::size_t amt)
{
	if (amt == 0)
		return;
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
/**Shift the array toward the significant side.  This function shifts bits.
\param arr1 The array.
\param amt The amount to shift (in bits).*/
template<typename T>
void ShiftSigB(cg::ArrayView<T>& arr1, std::size_t amt)
{
	ShiftSigB(arr1.Begin(), arr1.Size(), amt);
}
/**Shift the array toward the significant side.  This function shifts enire
units of T.
\param arr1 The array.
\param s1 The size of the array.
\param amt The amount to shift.*/
template<typename T>
void ShiftInsig(T* arr1, std::size_t s1, std::size_t amt)
{
	if (amt == 0)
		return;
	if (amt >= s1)
	{
		std::memset(arr1, 0, s1 * sizeof(T));
		return;
	}
	std::memmove(arr1, arr1 + amt, (s1 - amt) * sizeof(T));
	std::memset(arr1 + (s1 - amt), 0, amt * sizeof(T));
	return;
}
/**Shift the array toward the significant side.  This function shifts enire
units of T.
\param arr1 The array.
\param amt The amount to shift.*/
template<typename T>
void ShiftInsig(cg::ArrayView<T>& arr1, std::size_t amt)
{
	ShiftInsig(arr1.Begin(), arr1.Size(), amt);
}
/**Shift the array toward the significant side.  This function shifts bits.
\param arr1 The array.
\param s1 The size of the array.
\param amt The amount to shift.*/
template<typename T>
void ShiftInsigB(T* arr1, std::size_t s1, std::size_t amt)
{
	if (amt == 0)
		return;
	const static std::size_t TBits = (sizeof(T) * 8);
	std::size_t bytes = (amt / 8);
	std::size_t leftOver = amt % 8;
	if (bytes > 0)
		ShiftInsig<uint8_t>((uint8_t*)arr1, s1 * sizeof(T), bytes);
	T carry = 0;
	auto beg = arr1 + s1 - 1;
	auto end = arr1 - 1;
	for (; beg != end; --beg)
	{
		T nC = *beg << (TBits - leftOver);
		*beg >>= leftOver;
		*beg |= carry;
		carry = nC;
	}
	return;
}
/**Shift the array toward the significant side.  This function shifts bits.
\param arr1 The array.
\param amt The amount to shift.*/
template<typename T>
void ShiftInsigB(cg::ArrayView<T>& arr1, std::size_t amt)
{
	ShiftInsigB(arr1.Begin(), arr1.Size(), amt);
}
/**Determine the bit number of the most significant bit. Bit numbers start at 0
\param arr The array.
\param s The size of the array.
\return A number for which it, minus 1, is the number of the most significant
`1` bit. 0 means there was no 1 bit and the array is zero.*/
template<typename T>
std::size_t MSBNumber(const T* arr, std::size_t s)
{
	std::size_t ret = 0;
	for (std::size_t i = s - 1; i > 0; --i)
	{
		if (arr[i] == 0)
		{
			continue;
		}
		else
		{
			T t = arr[i];
			while (t)
			{
				t >>= 1;
				++ret;
			}
			return ret + (i * sizeof(T) * 8);
		}
	}
	if (*arr == 0)
		return 0;

	T t = *arr;
	while (t)
	{
		t >>= 1;
		++ret;
	}
	return ret;
}

}