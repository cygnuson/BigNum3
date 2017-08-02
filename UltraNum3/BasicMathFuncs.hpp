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

namespace cg
{


/**Array adding function.  The carry will propagate over
adjacent pointers up to the amount in s1.
\param arr1 The first array.
\param s1 the max size of arr1.
\param arr2 The second array.
\param s2 The max size of r2.
\return not relevent.*/
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
		if (doCarry && s1 != 1)
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

}