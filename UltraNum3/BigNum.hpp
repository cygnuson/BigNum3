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
#include "Num.hpp"

namespace cg {

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////SPLITTERS HERE/
///////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Internal_T, typename HalfType>
class RegularSplitter
{
	/**The datatype as a Num<T> object.*/
	using NumDataType = cg::Num<_Internal_T>;

	/**True if this thing is const.*/
	const static bool IAmConst = NumDataType::IAmConst;
	/**The storage type.*/
	using StoreType = typename NumDataType::StoreType;
	/**The basic type without any ref, ptr, or const.*/
	using BasicStoreType = typename NumDataType::BasicStoreType;
	/**Make sure the Num is a reference type.*/
	static_assert(std::is_reference<_Internal_T>::value,
		"Type T must be a reference type.");
protected:
	/**The type of thing to be the result of a split.*/
	using SplitterHalfType = HalfType;
	/**The type of thing to be the result of a split.*/
	using ConstSplitterHalfType = HalfType;
	/**Get the hi position ofthe number.*
	\return A reference or copy of the number depending on the constness or
	reference status of this number.
	\param first The first units to bein the halftype.
	\param amt The amount of units in the halftype
	(will do first + i, amt times)*/
	SplitterHalfType Hi(NumDataType* first, std::size_t amt)
	{
		return SplitterHalfType(*first);
	}
};

template<typename _Internal_T>
class NumSplitter
{
	/**The datatype as a Num<T> object.*/
	using NumDataType = cg::Num<_Internal_T>;

	/**True if this thing is const.*/
	const static bool IAmConst = NumDataType::IAmConst;
	/**The storage type.*/
	using StoreType = typename NumDataType::StoreType;
	/**The basic type without any ref, ptr, or const.*/
	using BasicStoreType = typename NumDataType::BasicStoreType;
	/**Make sure the Num is a reference type.*/
	static_assert(std::is_reference<_Internal_T>::value,
		"Type T must be a reference type.");
protected:
	/**The type of thing to be the result of a split.*/
	using SplitterHalfType = typename NumDataType::RefSelf;
	/**The type of thing to be the result of a split.*/
	using ConstSplitterHalfType = typename NumDataType::NonRefSelf;
	/**Get the hi position ofthe number.*
	\return A reference or copy of the number depending on the constness or
	reference status of this number.
	\param first The first units to bein the halftype.
	\param amt The amount of units in the halftype
	(will do first + i, amt times)*/
	SplitterHalfType Hi(NumDataType* first, std::size_t amt)
	{
		return SplitterHalfType(first->Get(0));
	}
};

template<typename _Internal_T, typename HalfType>
class HeapSplitter
{
	/**The datatype as a Num<T> object.*/
	using NumDataType = cg::Num<_Internal_T>;

	/**True if this thing is const.*/
	const static bool IAmConst = NumDataType::IAmConst;
	/**The storage type.*/
	using StoreType = typename NumDataType::StoreType;
	/**The basic type without any ref, ptr, or const.*/
	using BasicStoreType = typename NumDataType::BasicStoreType;
	/**Make sure the Num is a reference type.*/
	static_assert(std::is_reference<_Internal_T>::value,
		"Type T must be a reference type.");
protected:
	/**The type of thing to be the result of a split.*/
	using SplitterHalfType = HalfType;
	/**The type of thing to be the result of a split.*/
	using ConstSplitterHalfType = HalfType;
	/**Get the hi position ofthe number.*
	\return A reference or copy of the number depending on the constness or
	reference status of this number.
	\param first The first units to bein the halftype.
	\param amt The amount of units in the halftype
	(will do first + i, amt times)*/
	SplitterHalfType Hi(NumDataType* first, std::size_t amt)
	{
		return SplitterHalfType(*first);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////INTIMPL HERE//////
///////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Internal_T, std::size_t Units>
class BigNum : protected std::conditional_t <
	/*----*//*----*/Units == 0,
	/*----*//*----*/HeapSplitter<_Internal_T, BigNum<_Internal_T, Units>>,
	/*----*//*----*/std::conditional_t <
	/*----*//*----*//*----*/Units == 2,
	/*----*//*----*//*----*/NumSplitter<_Internal_T>,
	/*----*//*----*//*----*/RegularSplitter<_Internal_T,
	/*----*//*----*//*----*//*----*/BigNum<_Internal_T, Units / 2>
	/*----*//*----*//*----*/>
	/*----*//*----*/>
	>
{
public:
	/**The datatype as a Num<T> object.*/
	using NumDataType = cg::Num<_Internal_T>;

	/**True if this thing is const.*/
	const static bool IAmConst = NumDataType::IAmConst;
	/**The storage type.*/
	using StoreType = typename NumDataType::StoreType;
	/**The basic type without any ref, ptr, or const.*/
	using BasicStoreType = typename NumDataType::BasicStoreType;
	/**Make sure the Num is a reference type.*/
	static_assert(std::is_reference<_Internal_T>::value,
		"Type T must be a reference type.");
	/**A self reference type.*/
	using RefSelf = BigNum<std::remove_reference_t<StoreType>&, Units>;
	/**A self reference type.*/
	using NonRefSelf = BigNum<const std::remove_reference_t<StoreType>&, Units>;
	/**A self reference type.*/
	using Self = BigNum<_Internal_T, Units>;
	/**The type of splitter.*/
	using SplitterType = std::conditional_t <
		/*----*//*----*/Units == 0,
		/*----*//*----*/HeapSplitter<_Internal_T, BigNum<_Internal_T, Units>>,
		/*----*//*----*/std::conditional_t <
		/*----*//*----*//*----*/Units == 2,
		/*----*//*----*//*----*/NumSplitter<_Internal_T>,
		/*----*//*----*//*----*/RegularSplitter<_Internal_T,
		/*----*//*----*//*----*//*----*/BigNum<_Internal_T, Units / 2>
		/*----*//*----*//*----*/>
		/*----*//*----*/>
		>;
	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////CONSTRUCTORS HERE
	///////////////////////////////////////////////////////////////////////////
	/**Default for empty number.*/
	BigNum()
	{

	}
	/**Create with a single num in the low spot.
	\param t The number to emplace.*/
	template<typename T>
	BigNum(T&& t)
	{
		PushDigit(std::forward<T>(t));
	}
	/**Create with a bunch of nums.
	\param t The number to emplace.
	\param ts A bunch of numbers to emplace.*/
	template<typename T, typename...Ts>
	BigNum(T&& t, Ts&&...ts)
	{
		PushDigit(std::forward<T>(t));
		PushDigit(std::forward<T>(ts)...);
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
	///////////////////////////////////////////////////////////SPLITTERS HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Get the hi position ofthe number.*
	\return A reference or copy of the number depending on the constness or
	reference status of this number.
	\param first The first units to bein the halftype.
	\param amt The amount of units in the halftype
	(will do first + i, amt times)*/
	typename SplitterType::SplitterHalfType
		Hi()
	{
		auto sz = 0;
		sz = Units == 0 ? m_data.Size() / 2 : Units / 2;
		return SplitterType::Hi(Begin(), sz);
	}
private:
	/**Helper for adding numbers.
	\param t The number to emplace.
	\param ts A bunch of numbers to emplace.*/
	template<typename T, typename...Ts>
	void PushDigit(T&& t, Ts&&...ts)
	{
		PushDigit(std::forward<T>(t));
		PushDigit(std::forward<T>(ts)...);
	}
	/**Helper for adding numbers.
	\param t The number to emplace.*/
	template<typename T>
	void PushDigit(T&& t)
	{
		m_data.EmplaceBack(std::forward<T>(t));
	}
	/**The list to hold data*/
	cg::List<NumDataType, Units> m_data;
};


template class BigNum<uint16_t&, 2>;
template class BigNum<const uint16_t&, 2>;
template class BigNum<uint16_t&, 16>;
template class BigNum<const uint16_t&, 16>;

}