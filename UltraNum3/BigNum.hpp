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
////////////////////////////////////////////////////////////////////////////////////TAGS HERE//////
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////INTIMPL HERE//////
///////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Internal_T, std::size_t Units>
class BigNum
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

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////CONSTRUCTORS HERE
	///////////////////////////////////////////////////////////////////////////

	BigNum()
	{

	}
	template<typename T>
	BigNum(T&& t)
	{
		PushDigit(std::forward<T>(t));
	}
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
private:
	/**Helper for adding numbers.*/
	template<typename T, typename...Ts>
	void PushDigit(T&& t, Ts&&...ts)
	{
		PushDigit(std::forward<T>(t));
		PushDigit(std::forward<T>(ts)...);
	}
	/**Helper for adding numbers.*/
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