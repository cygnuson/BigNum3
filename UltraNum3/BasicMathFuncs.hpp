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

#include "Num.hpp"

namespace cg 
{


/**The basic math function pointer.  The carry will propagate over
adjacent pointers up to the amount in s1.
\param arr1 The first array.
\param s1 the max size of arr1.
\param arr2 The second array.
\param s2 The max size of r2.*/
template<typename T, typename U>
void AddNums(cg::Num<T>* arr1, std::size_t s1, const cg::Num<U>* arr2,
		std::size_t s2)
{
	auto & l = arr1->Get();
	auto & r = arr2->Get();
	l += r;
	if (l < r && s1 != 1)
	{
		cg::Num<U> t(1);
		AddNums(arr1 + 1, s1 - 1, &t, 1);
	}
	if (s2 != 1 && s1 != 1)
		AddNums(arr1 + 1, s1 - 1, arr2 + 1, s2 - 1);
}

}