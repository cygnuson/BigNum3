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

namespace cg {

/**Set bit flags on or off.
\param data The variable to turn bits on or off.
\param flag The flag to set.
\param set True to set on, false to set off.*/
template<typename T>
inline void SetFlag(T& data, const T& flag, bool set)
{
	if (set)
		data = (T)((std::size_t) data | (std::size_t) flag);
	else
		data = (T)((std::size_t) data & ~((std::size_t) flag));
}

/**Read the status of a flag.
\param data The data to read bits from.
\param flag The flag to check.
\return True if `flag` is on. False otherwise.*/
template<typename T>
bool ReadFlag(const T& data, const T& flag)
{
	return ((std::size_t) data & (std::size_t) flag) != 0;
}
/**Read the status of a flag.
\param data The data to read bits from.
\param flag The flag to check.
\return True if `flag` is on. False otherwise.*/
template<typename T>
bool AnyFlagsOn(const T& data, const T& flag)
{
	return (((std::size_t) data & (std::size_t) flag) != 0);
}
/**Read the status of a flag.
\param data The data to read bits from.
\param flag The flag to check.
\param flags Multi flags tocheck
\return True if `flag` is on. False otherwise.*/
template<typename T, typename...R>
bool AnyFlagsOn(const T& data, const T& flag, R&&... flags)
{
	return (((std::size_t) data & (std::size_t) flag) != 0)
		|| AnyFlagsOn(data, std::forward<R>(flags)...);
}
/**Read the status of a flag.
\param data The data to read bits from.
\param flag The flag to check.
\return True if `flag` is on. False otherwise.*/
template<typename T>
bool AllFlagsOn(const T& data, const T& flag)
{
	return (((std::size_t) data & (std::size_t) flag) != 0);
}
/**Read the status of a flag.
\param data The data to read bits from.
\param flag The flag to check.
\param flags Multi flags tocheck
\return True if `flag` is on. False otherwise.*/
template<typename T, typename...R>
bool AllFlagsOn(const T& data, const T& flag, R&&... flags)
{
	return (((std::size_t) data & (std::size_t) flag) != 0)
		&& AllFlagsOn(data, std::forward<R>(flags)...);
}

/**Check if a bet is on or off.
\param v The variable to check.
\param bitnum The bit to check starting from 0.
\return True if the bit is on, false if it is off.
\throws Throws invalid_argument if `bitnum` < 0 or >= sizeof(T)*8 */
template<typename T>
bool CheckBit(const T& v, int bitnum)
{
	if (bitnum < 0 || bitnum >= (sizeof(T) * 8))
		throw std::invalid_argument("The bit number is invalid.");
	return ((T(1) << bitnum) & v) == 1;
}

/**An object helper for LCM pairs.*/
template<typename T>
struct LCMPair
{
	/**The number when multiplied to the first parameter of the LCM function
	will produce the lowest common multiple of both parameters.*/
	T first;
	/**The number when multiplied to the second parameter of the LCM function
	will produce the lowest common multiple of both parameters.*/
	T second;
};

/* Find  the GCD of two numbers. Neither may be zero.

my implementation of the Euclidean algorithm to find the GCD.
https://en.wikipedia.org/wiki/Euclidean_algorithm#Background:_greatest_common_divisor
\param first The first number.
\param second The second number.
*/
template<typename T>
T GCD(T first, T second)
{
	if (first < 0)
		first = -first;
	if (second < 0)
		second = -second;
	if (first == 1 || second == 1)
		return 1;
	if (first == second)
		return first;
	auto larger = first > second ? first : second;
	auto lesser = first > second ? second : first;
	T remainder = 1;
	while (remainder != 0)
	{
		remainder = larger % lesser;
		if (remainder == 0)
			return lesser;
		larger = lesser;
		lesser = remainder;
	}
	return 1;
}

/**Get the least common multiple of a pair of numbers.
\param first The first number.
\param second The second number.
\return The number that is the least common multiple.*/
template<typename T>
T LCM(const T& first, const T& second)
{
	LCMPair lcmresult;
	T gcd = GCD<T>(first, second);
	if (gcd == 1)
	{
		lcmresult.first = second;
		lcmresult.second = first;
		return lcmresult;
	}
	lcmresult.first = (second / gcd);
	lcmresult.second = (first / gcd);
	return lcmresult;
}

/**Make a number into twos compliment.
\param n The number to change. Will be changed.*/
template<typename DataType>
void TwoCompInPlace(DataType& n)
{
	n = (~n) + 1;
}
/**Make a number into twos compliment.
\param n The number to change.
\return The new number.*/
template<typename DataType>
DataType TwoComp(const DataType& n)
{
	return (~n) + 1;
}


}