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
#include <algorithm>

#include "Helpers.hpp"
#include "Type.hpp"
#include "BasicBits.hpp"
#include "ArrayView.hpp"

namespace cg
{
/**Set an array to zero.
\param arr The array.
\param s The size of the array.*/
template<typename T>
inline void ZeroOut(T* arr, std::size_t s)
{
	std::memset(arr, 0, s * sizeof(T));
}
/**Set an array to zero.
\param arr The array.*/
template<typename T>
inline void ZeroOut(cg::ArrayView<T>& arr)
{
	arr.ZeroOut();
}

/**Determine if an array is zero.
\param arr The array.
\param s The size of the array.
\return True if the array is zero.*/
template<typename T>
inline bool IsZero(const T* arr, std::size_t s)
{
	for (std::size_t i = 0; i < s; ++i)
		if (arr[i] != 0)
			return false;
	return true;
}
/**Determine if an array is zero.
\param arr The array.
\return True if the array is zero.*/
template<typename T>
inline bool IsZero(const cg::ArrayView<T>& arr)
{
	return IsZero(arr.Begin(), arr.Size());
}
/**Determine if an array is one.
\param arr The array.
\param s The size of the array.
\return True if the array is zero.*/
template<typename T>
inline bool IsOne(const T* arr, std::size_t s)
{
	return (*arr == 1) && IsZero(arr + 1, s - 1);
}
/**Determine if an array is one.
\param arr The array.
\return True if the array is zero.*/
template<typename T>
inline bool IsOne(const cg::ArrayView<T>& arr)
{
	return IsOne(arr.Begin(), arr.Size());
}
/**Compare two objects.  This function assumes that there are no leading zeros
that do not effect the value of the function.
\param arr1 The first array.
\param s1 the max size of arr1.
\param arr2 The second array.
\param s2 The max size of r2.
\return -1 if arr1 < arr2, 0 if arr1 == arr2, 1 if arr1 > arr2.*/
template<typename T>
inline int CompareArray(const T* arr1, std::size_t s1, const T* arr2,
	std::size_t s2)
{
	if (s1 == s2)
	{
		auto beg1 = arr1 + (s1 - 1);
		auto beg2 = arr2 + (s2 - 1);
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
	else if (s1 < s2)
		/*arr1 < arr2*/
		return -1;
	else //(s1 > s2)
		/*arr1 > arr2*/
		return 1;
}

/**Compare two objects.  This function assumes that there are no leading zeros
that do not effect the value of the function.
\param arr1 The first array.
\param arr2 The second array.
\return -1 if arr1 < arr2, 0 if arr1 == arr2, 1 if arr1 > arr2.*/
template<typename T>
inline int CompareArray(const cg::ArrayView<T>& arr1,
	const cg::ArrayView<T>& arr2)
{
	return CompareArray(arr1.Begin(), arr1.Size(), arr2.Begin(), arr2.Size());
}

/**Array adding function.  The carry will propagate over
adjacent pointers up to the amount in s1.
\param arr1 The first array.
\param s1 the max size of arr1.
\param arr2 The second array.
\param s2 The max size of r2.
\return false always.*/
template<typename T>
inline bool AddArray(T* arr1, std::size_t s1, const T* arr2, std::size_t s2)
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
/**Array adding function.  The carry will propagate over
adjacent pointers up to the amount in s1.
\param arr1 The first array.
\param arr2 The second array.
\return false always.*/
template<typename T>
inline bool AddArray(cg::ArrayView<T>& arr1, const cg::ArrayView<T>& arr2)
{
	return AddArray(arr1.Begin(), arr1.Size(), arr2.Begin(), arr2.Size());
}
/**Add a single num to the array.
\param arr The array.
\param s The size of the array.
\param n A number to add to the array.*/
template<typename T>
inline void AddArray(T* arr, std::size_t s, const T& n)
{
	AddArray(arr, s, &n, 1);
}
/**Add a single num to the array.
\param arr The array.
\param n A number to add to the array.*/
template<typename T>
inline void AddArray(cg::ArrayView<T>& arr, const T& n)
{
	AddArray(arr.Begin(), arr.Size(), n);
}
/**The sub function.  The borrow will propagate over
adjacent pointers up to the amount in s1.
\param arr1 The first array.
\param s1 the max size of arr1.
\param arr2 The second array.
\param s2 The max size of r2.
\return True if the result went below zero. False if its still > 0.*/
template<typename T>
inline bool SubArray(T* arr1, std::size_t s1, const T* arr2, std::size_t s2)
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

/**The sub function.  The borrow will propagate over
adjacent pointers up to the amount in s1.
\param arr1 The first array.
\param arr2 The second array.
\return True if the result went below zero. False if its still > 0.*/
template<typename T>
inline bool SubArray(cg::ArrayView<T>& arr1, const cg::ArrayView<T>& arr2)
{
	return SubArray(arr1.Begin(), arr1.Size(), arr2.Begin(), arr2.Size());
}
/**Sub a single num from the array.
\param arr The array.
\param s The size of the array.
\param n A number to sub from the array.*/
template<typename T>
inline void SubArray(T* arr, std::size_t s, const T& n)
{
	SubArray(arr, s, &n, 1);
}
/**Sub a single num from the array.
\param arr The array.
\param n A number to sub from the array.*/
template<typename T>
inline void SubArray(cg::ArrayView<T>& arr, const T& n)
{
	SubArray(arr.Begin(), arr.Size(), n);
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
inline bool MulArray(T* arr1, std::size_t s1, const T* arr2, std::size_t s2)
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
	delete[] tArr;
	return false;
}
/**The mult function.  The borrow will propagate over
adjacent pointers up to the amount in s1.  Should be called with T = a type
that is half the size of the actual type.
\param arr1 The first array.
\param arr2 The second array.
\return false always.*/
template<typename T>
inline bool MulArray(cg::ArrayView<T>& arr1, const cg::ArrayView<T>& arr2)
{
	return MulArray(arr1.Begin(), arr1.Size(), arr2.Begin(), arr2.Size());
}
/**Iteger power function.
\param num A reference to the number to apply to the power.
\param exp The exponent to apply.
\return A reference to num*/
template<typename T>
inline T& PowInPlace(T& num, std::size_t exp)
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
/**Shift an array to its maximum value while staying less than some other num.
\param arr1 The array that will be shifted.
\param s1 The size of arr1.
\param arr2 The second array that will remain larger than arr1.
\param s2 The size of the second array.
\param max The total amount of shifts that may be made.
\return The amount of shifts made on arr1.*/
template<typename T>
inline std::size_t MaximumShift(T* arr1, std::size_t s1, const T* arr2,
	std::size_t s2, std::size_t max)
{
	if (max == 0)
		return 0;
	std::size_t msb_1 = MSBNumber(arr1, s1);
	std::size_t msb_2 = MSBNumber(arr2, s2);
	if (msb_1 < msb_2)
	{
		std::size_t shiftAmt = (msb_2 - 1) - msb_1;
		shiftAmt = shiftAmt <= max ? shiftAmt : max;
		ShiftSigB(arr1, s1, shiftAmt);
		return shiftAmt;
	}
	else if (msb_1 > msb_2)
	{
		std::size_t shiftAmt = (msb_1)-(msb_2 - 1);
		shiftAmt = shiftAmt <= max ? shiftAmt : max;
		ShiftInsigB(arr1, s1, shiftAmt);
		return shiftAmt;
	}
	/*both are the same msb*/
	std::size_t shiftAmt = 1;
	shiftAmt = shiftAmt <= max ? shiftAmt : max;
	ShiftInsigB(arr1, s1, shiftAmt);
	return shiftAmt;
}
/**Shift an array to its maximum value while staying less than some other num.
\param arr1 The array that will be shifted.
\param arr2 The second array that will remain larger than arr1.
\param max The total amount of shifts that may be made.
\return The amount of shifts made on arr1.*/
template<typename T>
inline std::size_t MaximumShift(cg::ArrayView<T>& arr1,
	const cg::ArrayView<T>& arr2, std::size_t max)
{
	return MaximumShift(arr1.Begin(), arr1.Size(),
		arr2.Begin(), arr2.Size(), max);
}
/**The basic division function.  This function assumes there are no MSB zeros.
\param arr1 The first dividend array.  Will be the answer after the function
returnes.
\param s1 The size of the first array.
\param arr2 The second divisor array.
\param s2 The size of the second array.
\param arr3 The third array that will hold the modulo of the operation. If its
nullptr (or 0) it will be ignored.  If its not false, it must be the same size
as s1.*/
template<typename T>
inline void DivArray_Shift(T* arr1, std::size_t s1, const T* arr2,
	std::size_t s2, T* arr3)
{
	static_assert(sizeof(T) > 1, "T must be at least 2 bytes long.");
	const static T TMax = std::numeric_limits<T>::max();
	const std::size_t s1T = s1 * sizeof(T);
	if ((s1 == 1) && (s2 == 1))
	{
		if (arr3)
			*arr3 = *arr1;
		*arr1 /= *arr2;
		if (arr3)
			*arr3 %= *arr2;
		return;
	}
	if (IsZero(arr2, s2))
		throw std::invalid_argument("Divisor is zero.");
	if (IsOne(arr2, s2))
	{
		if (arr3)
			ZeroOut(arr3, s1);
		return;
	}
	bool cleanArr3 = false;
	if (!arr3)
	{
		arr3 = new T[s1];
		cleanArr3 = true;
	}
	/*move the contents of array-1 into array-3 so that it contains the answer
	when all is said and done.*/
	std::memmove(arr3, arr1, s1 * sizeof(T));
	/**arr1 will be the counter (and then the answer when done).*/
	std::memset(arr1, 0, s1 * sizeof(T));
	T* answerCounter = arr1;
	T* dividend = arr3;
	T* divisor = new T[s1]();
	std::memmove(divisor, arr2, s2 * sizeof(T));
	auto compareResult = CompareArray(dividend, s1, divisor, s1);
	if (compareResult == 0)
	{/*Both oprands are the same, answer = 1, modulo = 0 (already moved
	 to the right spots by this line).*/
		answerCounter[0] = 1;
		std::memset(dividend, 0, s1T);
	}
	/*arr1 is larger, can do some math here.*/
	auto shifted = MaximumShift(divisor, s1, dividend, s1, TMax);
	compareResult = CompareArray(dividend, s1, divisor, s1);
	while (compareResult == 1)
	{
		SubArray(dividend, s1, divisor, s1);
		AddArray<T>(answerCounter, s1, 1);
		compareResult = CompareArray(dividend, s1, divisor, s1);
		if (compareResult == 0)
		{
			std::memset(dividend, 0, s1T);
			AddArray<T>(answerCounter, s1, 1);
			ShiftSigB(answerCounter, s1, shifted);
			break;
		}
		else if (compareResult == -1)
		{/*divident is < divisor.*/
			if (shifted == 0)
				break;
			auto unshift
				= MaximumShift(divisor, s1, dividend, s1, shifted);
			shifted -= unshift;
			ShiftSigB(answerCounter, s1, unshift);
			if (shifted > 0)
				/*dividend will be > divisor here. Manually set the result.*/
				compareResult = 1;
			else
			{/*shifted is zero, must do real comparison.*/
				compareResult = CompareArray(dividend, s1, divisor, s1);
				if (compareResult == 0)
				{
					std::memset(dividend, 0, s1T);
					AddArray<T>(answerCounter, s1, 1);
					ShiftSigB(answerCounter, s1, shifted);
					break;
				}
			}
		}
	}
	if (cleanArr3)
		delete[] arr3;
	delete[] divisor;
}

/**The basic division function.  This function assumes there are no MSB zeros.
\param arr1 The first dividend array.
\param arr2 The second divisor array.
\param arr3 The third array that will hold the modulo of the operation. Iff its
nullptr (or 0) it will be ignored.  If its not false, it must be the same size
as s1.*/
template<typename T>
inline void DivArray_Shift(cg::ArrayView<T>& arr1, const cg::ArrayView<T>& arr2,
	cg::ArrayView<T>& arr3)
{
	if (arr1.Size() != arr3.Size())
		throw std::runtime_error(
			"The size of arr1 and arr2 must be the same.");
	DivArray_Shift(arr1.Begin(), arr1.Size(), arr2.Begin(),
		arr2.Size(), arr3.Begin());
}

}