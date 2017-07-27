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

#include <cstdint>
#include <stdexcept>

#if _DEBUG
#include <iostream>
#endif


namespace cg {


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////NON REF HERE////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/**number storage.
The two opts for T are:
....const type&  :Will be a non reference type, which takes const& ctor params.
....type&        :Will be a ref type that takes ref ctor parameters.
\tparam T The type of parameter to take int the CTOR. 
---If T=cosnt type& then it will have stack data and copy or move a const T& 
or T&&.
---If T= type& it will have a reference to another object as data and will 
take a referenc (non const) in the ctor.
*/
template<typename _Internal_T>
class Num
{
public:
	/**The storage type.*/
	using StoreType = std::conditional_t<
		/*If T is const:*/
		std::is_const<std::remove_reference_t<_Internal_T>>::value,
		/*Then data type will be non-ref, non-cont.*/
		std::remove_const_t<std::remove_reference_t<_Internal_T>>,
		/**Otherwise just use T.*/
		_Internal_T
	>;
	/**The basic type without any ref, ptr, or const.*/
	using BasicStoreType
		= std::remove_const_t<std::remove_reference_t<StoreType>>;
	/**Make sure the Num is a reference type.*/
	static_assert(std::is_reference<_Internal_T>::value,
		"Type T must be a reference type.");
	/**A self reference type.*/
	using RefSelf = Num<std::remove_reference_t<StoreType>&>;
	/**A self reference type.*/
	using NonRefSelf = Num<const std::remove_reference_t<StoreType>&>;
	/**A self reference type.*/
	using Self = Num<_Internal_T>;
	/**Create the num storage with a reference.*/
	Num(_Internal_T&& num = 0)
		:m_data(std::forward<_Internal_T>(num)) {}
	/**Copy ctor
	\param other The other thing to copy.*/
	Num(const Num& other)
		:m_data(other.m_data) {}

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////ITERATORS HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Get the begin iterator.
	\return An iterator to the front of the list.*/
	auto* Begin()
	{
		return &m_data;
	}
	/**Get the begin iterator.
	\return An iterator to the front of the list.*/
	const auto* Begin() const
	{
		return &m_data;
	}
	/**Get an iterator to the end+1 of the list.
	\return An iterator to End+1*/
	auto* End()
	{
		return  (&m_data) + 1;
	}
	/**Get an iterator to the end+1 of the list.
	\return An iterator to End+1*/
	const auto* End() const
	{
		return (&m_data) + 1;
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////ACCESSORS HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Direct access to the data.
	\return The data as a T&.*/
	operator StoreType& ()
	{
		return Get();
	}
	/**Direct access to the data.
	\return The data as a const T&.*/
	operator const StoreType&() const
	{
		return Get();
	}
	/**Direct access to the data.
	\param i A compatibility param. Must be 1.
	\return The data as a T&.*/
	StoreType& Get(std::size_t i = 1)
	{
		if (i != 1)
			throw std::invalid_argument("Paramter out of bounds.");
		return m_data;
	}
	/**Direct access to the data.
	\param i A compatibility param. Must be 1.
	\return The data as a T&.*/
	const StoreType& Get(std::size_t i = 1) const
	{
		if (i != 1)
			throw std::invalid_argument("Paramter out of bounds.");
		return m_data;
	}
	/**Get an element.
	\param i A compatibility param. Must be 1.
	\return The data as a T&.*/
	auto& operator[](std::size_t i)
	{
		if (i != 1)
			throw std::invalid_argument("Paramter out of bounds.");
		return Get();
	}
	/**Get an element.
	\param i A compatibility param. Must be 1.
	\return The data as a T&.*/
	const auto& operator[](std::size_t i) const
	{
		if (i != 1)
			throw std::invalid_argument("Paramter out of bounds.");
		return Get();
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////UTILITIES HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Get a reference to the internal data of this object.
	\return A modifiable reference to the data of this object.*/
	RefSelf GetReference()
	{
		return RefSelf(m_data);
	}

	/**Set the value of the data.
	\param n The number to set.*/
	void Set(const BasicStoreType& n)
	{
		m_data = n;
	}
	/**Set the value of the data.
	\param n The number to set.*/
	void Set(BasicStoreType&& n)
	{
		m_data = std::move(n);
	}
	/**Get the size of the storage.
	\return The amount of elements.*/
	auto Size() const
	{
		return 1;
	}
	/**Determine if this is zero.
	\return True if this is zero.*/
	bool IsZero() const
	{
		return Get() == 0;
	}
	/**Create a copy of this thing that cuts all reference.
	\return A copy of the object with copied data and no references.*/
	NonRefSelf HardCopy() const
	{
		return NonRefSelf(m_data);
	}
private:
	/**The internal data.*/
	StoreType m_data;
};

template class Num<const uint64_t&>;

}