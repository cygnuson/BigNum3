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

#include "List.hpp"
#include "BasicMathFuncs.hpp"

namespace cg {

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////INTIMPL HERE//////
///////////////////////////////////////////////////////////////////////////////////////////////////

template<typename DataType, std::size_t Units>
class BigNum
{
public:
	/**Require even number of digits.*/
	static_assert(Units % 2 == 0, "Must have even size.");
	/**A self reference type.*/
	using Self = BigNum<DataType, Units>;

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////CONSTRUCTORS HERE
	///////////////////////////////////////////////////////////////////////////

	/**Create with a premade list.
	\param l The list.*/
	BigNum(cg::List<DataType, Units>&& l)
		:m_data(std::move(l))
	{

	}
	/**Create with a premade list.
	\param l The list.*/
	BigNum(const cg::List<DataType, Units>& l)
		:m_data(std::move(l.Copy()))
	{

	}
	/**Default for empty number.*/
	BigNum()
	{

	}
	/**Create with a bunch of nums.
	\param t The number to emplace.
	\param ts A bunch of numbers to emplace.*/
	template<typename...Ts>
	BigNum(Ts&&...ts)
	{
		if (sizeof...(Ts) % 2 != 0)
			throw std::invalid_argument("Must have even number of digits.");
		char dummy[sizeof...(Ts)] = { (m_data.PushBack(ts), 0)... };
	}

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////ITERATORS HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Get the begin iterator.
	\return An iterator to the front of the list.*/
	auto* Begin()
	{
		return m_data.Begin();
	}
	/**Get the begin iterator.
	\return An iterator to the front of the list.*/
	const auto* Begin() const
	{
		return m_data.Begin();
	}
	/**Get an iterator to the end+1 of the list.
	\return An iterator to End+1*/
	auto* End()
	{
		return m_data.End();
	}
	/**Get an iterator to the end+1 of the list.
	\return An iterator to End+1*/
	const auto* End() const
	{
		return m_data.End();
	}

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////OPERATORS HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Do a math operation.
	\param r The thing to operate on this with.
	\return A reference to this.*/
	Self& operator+=(const DataType& r)
	{
		(mf_addFunc)(Begin(), Size(), &r, 1);
		return *this;
	}
	/**Do a math operation.
	\param r The thing to operate on this with.
	\return A reference to this.*/
	template<typename U, std::size_t S>
	Self& operator+=(const BigNum<U,S>& r)
	{
		(mf_addFunc)(Begin(), Size(), r.Begin(), r.Size());
		return *this;
	}
	/**Do a math operation.
	\param r The thing to operate on this with.
	\return A reference to this.*/
	Self& operator-=(const DataType& r)
	{
		(mf_subFunc)(Begin(), Size(), &r, 1);
		return *this;
	}
	/**Do a math operation.
	\param r The thing to operate on this with.
	\return A reference to this.*/
	template<typename U, std::size_t S>
	Self& operator-=(const BigNum<U, S>& r)
	{
		(mf_subFunc)(Begin(), Size(), r.Begin(), r.Size());
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////ACCESSORS HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Add an array of thing to the number.
	\param arr The array to add.
	\param aSize The size of the array.*/
	void AddArray(const DataType* arr, std::size_t aSize)
	{
		auto end = arr + aSize;
		for (; arr != end; ++arr)
			m_data.PushBack(*arr);
	}

	/**Direct access to the data.
	\param i A compatibility param. Must be 1.
	\return The data as a DataType&.*/
	DataType& Get(std::size_t i)
	{
		return m_data.Get(i);
	}
	/**Direct access to the data.
	\param i A compatibility param. Must be 1.
	\return The data as a DataType&.*/
	const DataType& Get(std::size_t i) const
	{
		return m_data.Get(i);
	}
	/**Get an element.
	\param i A compatibility param. Must be 0.
	\return The data as a T&.*/
	auto& operator[](std::size_t i)
	{
		return Get(i);
	}
	/**Get an element.
	\param i A compatibility param. Must be 0.
	\return The data as a T&.*/
	const auto& operator[](std::size_t i) const
	{
		return Get(i);
	}

	/**Set the value of the data.
	\param i The index to set.
	\param n The number to set.*/
	void Set(std::size_t i, const DataType& n)
	{
		m_data.Get(i) = n;
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////UTILITIES HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Get the size of the storage.
	\return The amount of elements.*/
	auto Size() const
	{
		return m_data.Size();
	}
	/**Get the amount of MSD-side-Zeros.
	\return The amount of zeros that dont effect value.*/
	std::size_t MSDZeros() const
	{
		std::size_t i = m_data.Size()-1;
		while (m_data.Get(i) == 0)
			--i;
		return i;
	}
	/**Get the amount of digits excluding the MSD side zeros (zeroes that 
	dont change value).
	\return The amount of non-MSD-zero digits.*/
	std::size_t RealSize()const
	{
		return Size() - MSDZeros();
	}
	/**Determine if this is zero.
	\return True if this is zero.*/
	bool IsZero() const
	{
		return RealSize() == 0;
	}
	/**Swap the value of this and another thing.
	\param other The other thing to swap with.*/
	void Swap(Self& other)
	{
		auto t = std::move(other.m_data);
		other.m_data = std::move(m_data);
		m_data = std::move(t);
	}
	/**Apply twos compliment to this numbe.*/
	void MakeTwoComp()
	{
		auto beg = Begin();
		auto end = End();
		for (; beg != end; ++beg)
			cg::TwoCompInPlace(*beg);
	}
	/**Apply compliment to this numbe.*/
	void MakeComp()
	{
		auto beg = Begin();
		auto end = End();
		for (; beg != end; ++beg)
			*beg = ~(*beg);
	}
private:
	/**The list to hold data*/
	cg::List<DataType, Units> m_data;
private:
	/**The type of math function pointers.*/
	using MathFuncPtr 
		= bool(*)(DataType*, std::size_t, const DataType*, std::size_t);
	/**The function to call for adding the arrays*/
	MathFuncPtr mf_addFunc = &cg::AddArray;
	/**The function to call for subtracting the arrays*/
	MathFuncPtr mf_subFunc = &cg::SubArray;
};

/**Get a pointer to a number as a lesser type.
\param n The number to access.
\return A pointer to the number `n` but as a lesser type.
\tparam T the type change too.
\tparam U The thing to be `n`*/
template<typename T, typename U>
T* AsArray(U& n)
{
	return (T*)&n;
}
/**Get a pointer to a number as a lesser type.
\param n The number to access.
\return A pointer to the number `n` but as a lesser type.
\tparam T the type change too.
\tparam U The thing to be `n`*/
template<typename T, typename U>
const T* AsArray(const U& n)
{
	return (const T*)&n;
}


template class BigNum<uint16_t, 2>;
template class BigNum<uint16_t, 16>;

}