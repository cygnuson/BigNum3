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
	/**Require even number of digits.*/
	static_assert(Units % 2 == 0, "Must have even size.");
	/**A self reference type.*/
	using RefSelf = BigNum<std::remove_reference_t<StoreType>&, Units>;
	/**A self reference type.*/
	using NonRefSelf = BigNum<const std::remove_reference_t<StoreType>&, Units>;
	/**A self reference type.*/
	using Self = BigNum<_Internal_T, Units>;

	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////CONSTRUCTORS HERE
	///////////////////////////////////////////////////////////////////////////

	/**Create with a premade list.
	\param l The list.*/
	BigNum(cg::List<NumDataType, Units>&& l)
		:m_data(std::move(l))
	{

	}
	/**Default for empty number.*/
	BigNum()
	{

	}
	/**Create with a single num in the low spot.
	\param t The number to emplace.
	\param t2 The second number to emplace.*/
	template<typename T>
	BigNum(T&& t, T&& t2)
	{
		PushDigit(std::forward<T>(t));
		PushDigit(std::forward<T>(t2));
	}
	/**Create with a bunch of nums.
	\param t The number to emplace.
	\param ts A bunch of numbers to emplace.*/
	template<typename T, typename...Ts>
	BigNum(T&& t, Ts&&...ts)
	{
		if (sizeof...(Ts) % 2 == 0)
			throw std::invalid_argument("Must have even number of digits.");
		PushDigit(std::forward<T>(t));
		PushDigit(std::forward<Ts>(ts)...);
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



	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////ACCESSORS HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Direct access to the data.
	\param i A compatibility param. Must be 1.
	\return The data as a T&.*/
	NumDataType& Get(std::size_t i)
	{
		if (i >= m_data.Size())
			throw std::invalid_argument("Paramter out of bounds.");
		return m_data.Get(i);
	}
	/**Direct access to the data.
	\param i A compatibility param. Must be 1.
	\return The data as a T&.*/
	const NumDataType& Get(std::size_t i) const
	{
		if (i >= m_data.Size())
			throw std::invalid_argument("Paramter out of bounds.");
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

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////UTILITIES HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Get the size of the storage.
	\return The amount of elements.*/
	auto Size() const
	{
		return m_data.Size();
	}
private:
	/**Helper for adding numbers.
	\param t The number to emplace.
	\param ts A bunch of numbers to emplace.*/
	template<typename T, typename...Ts>
	void PushDigit(T&& t, Ts&&...ts)
	{
		PushDigit(std::forward<T>(t));
		PushDigit(std::forward<Ts>(ts)...);
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

/**Helper for determining the return types of BigNum splitters.*/
template<typename T, std::size_t S>
class BigNumHelper
{
public:
	const static bool DoNum = S == 2;
	const static bool DoHeap = S == 0;
	const static std::size_t NewSize = S == 0 ? 0 : S / 2;
	using Type = std::conditional_t<
		/*-*//*-*//*-*/DoNum,
		/*-*//*-*//*-*/Num<T>,
		/*-*//*-*//*-*/std::conditional_t<DoHeap,
		/*-*//*-*//*-*//*-*/BigNum<T, 0>,
		/*-*//*-*//*-*//*-*/BigNum<T, S / 2>
		/*-*//*-*//*-*/>
		/*-*//*-*/>;
};

/**Get the hi part.
\param n The num for which to get the part.
\return the apropriate part of the num `n`.*/
template<typename T, std::size_t S>
auto Hi(BigNum<T, S>& n)
{
	using RT = typename BigNumHelper<T, S>::Type;
	cg::List<Num<T>, BigNumHelper<T, S>::NewSize> l;
	auto sz = n.Size();
	for (std::size_t i = n.Size() / 2; i < sz; ++i)
		l.PushBack(n.Get(i).GetReference());
	return RT(std::move(l));
}
/**Get the hi part.
\param n The num for which to get the part.
\return the apropriate part of the num `n`.*/
template<typename T, std::size_t S>
auto Hi(const BigNum<T, S>& n)
{
	using CT = const std::decay_t<T>&;
	using RT = typename BigNumHelper<CT, S>::Type;
	cg::List<Num<CT>, BigNumHelper<CT, S>::NewSize> l;
	auto sz = n.Size();
	for (std::size_t i = n.Size() / 2; i < sz; ++i)
		l.PushBack(n.Get(i).GetReference());
	return RT(std::move(l));
}
/**Get the lo part.
\param n The num for which to get the part.
\return the apropriate part of the num `n`.*/
template<typename T, std::size_t S>
auto Lo(BigNum<T, S>& n)
{
	using RT = typename BigNumHelper<T, S>::Type;
	cg::List<Num<T>, BigNumHelper<T, S>::NewSize> l;
	auto sz = n.Size();
	for (std::size_t i = 0; i < sz / 2; ++i)
		l.PushBack(n.Get(i).GetReference());
	return RT(std::move(l));
}
/**Get the lo part.
\param n The num for which to get the part.
\return the apropriate part of the num `n`.*/
template<typename T, std::size_t S>
auto Lo(const BigNum<T, S>& n)
{
	using CT = const std::decay_t<T>&;
	using RT = typename BigNumHelper<CT, S>::Type;
	cg::List<Num<CT>, BigNumHelper<CT, S>::NewSize> l;
	auto sz = n.Size();
	for (std::size_t i = 0; i < sz / 2; ++i)
		l.PushBack(n.Get(i).GetReference());
	return RT(std::move(l));
}


template class BigNum<uint16_t&, 2>;
template class BigNum<const uint16_t&, 2>;
template class BigNum<uint16_t&, 16>;
template class BigNum<const uint16_t&, 16>;

}