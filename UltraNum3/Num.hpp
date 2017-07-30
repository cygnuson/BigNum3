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

#include "Type.hpp"
#include "Endian.hpp"


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
	/**True if this thing is const.*/
	const static bool IAmConst
		= std::is_const<std::remove_reference_t<_Internal_T>>::value;
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
	using BasicStoreType = std::decay_t<_Internal_T>;
	/**Make sure the Num is a reference type.*/
	static_assert(std::is_reference<_Internal_T>::value,
		"Type T must be a reference type.");
	/**A self reference type.*/
	using RefSelf = Num<std::remove_reference_t<StoreType>&>;
	/**A self reference type.*/
	using NonRefSelf = Num<const std::remove_reference_t<StoreType>&>;
	/**A self reference type.*/
	using Self = Num<_Internal_T>;
	/**The demoted type.*/
	using DemotedBaseType = typename cg::DemoteType<BasicStoreType>::Type;
	///////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////CONSTRUCTORS HERE//
	///////////////////////////////////////////////////////////////////////////
	/*Default*/
	Num() {}
	/**Create the num storage with a reference.*/
	Num(_Internal_T&& num)
		:m_data(std::forward<_Internal_T>(num)) {}
	/**Copy ctor
	\param other The other thing to copy.*/
	Num(const Num<_Internal_T>& other)
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
	\param i A compatibility param. Must be 1.
	\return The data as a T&.*/
	BasicStoreType& Get(std::size_t i)
	{
		if (i != 0)
			throw std::invalid_argument("Paramter out of bounds.");
		return m_data;
	}
	/**Direct access to the data.
	\param i A compatibility param. Must be 1.
	\return The data as a T&.*/
	const BasicStoreType& Get(std::size_t i) const
	{
		if (i != 0)
			throw std::invalid_argument("Paramter out of bounds.");
		return m_data;
	}
	/**Get an element.
	\param i A compatibility param. Must be 0.
	\return The data as a T&.*/
	auto& operator[](std::size_t i)
	{
		if (i != 0)
			throw std::invalid_argument("Paramter out of bounds.");
		return Get(i);
	}
	/**Get an element.
	\param i A compatibility param. Must be 0.
	\return The data as a T&.*/
	const auto& operator[](std::size_t i) const
	{
		if (i != 0)
			throw std::invalid_argument("Paramter out of bounds.");
		return Get(i);
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
	/**Get a reference to the internal data of this object.
	\return A modifiable reference to the data of this object.*/
	NonRefSelf GetReference() const
	{
		return NonRefSelf(m_data);
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
		return Get(0) == 0;
	}
	/**Create a copy of this thing that cuts all reference.
	\return A copy of the object with copied data and no references.*/
	NonRefSelf HardCopy() const
	{
		return NonRefSelf(m_data);
	}
	/**Swap the value of this and another thing.
	\param other The other thing to swap with.*/
	template<typename U>
	void Swap(Num<U>& other)
	{
		auto t = m_data;
		m_data = other.m_data;
		other.m_data = t;
	}

private:
	/**Add the NumFriend for the Splitter access.*/
	friend class NumFriend;
	/**Get the upper part of the number.
	\return A reference to the upper part of the number.*/
	auto Hi()
	{
		if (!cg::Endian::little)
			return Num<DemotedBaseType&>
			(*((DemotedBaseType*)&m_data));
		else
			return Num<DemotedBaseType&>
			(*(((DemotedBaseType*)&m_data) + 1));
	}
	/**Get the upper part of the number.
	\return A reference to the upper part of the number.*/
	const auto Hi() const
	{
		if (!cg::Endian::little)
			return Num<const DemotedBaseType&>
			(*((DemotedBaseType*)&m_data));
		else
			return Num<const DemotedBaseType&>
			(*(((DemotedBaseType*)&m_data) + 1));
	}
	/**Get the lower part of the number.
	\return A reference to the lower part of the number.*/
	auto Lo()
	{
		if (cg::Endian::little)
			return Num<DemotedBaseType&>
			(*((DemotedBaseType*)&m_data));
		else
			return Num<DemotedBaseType&>
			(*(((DemotedBaseType*)&m_data) + 1));
	}
	/**Get the lower part of the number.
	\return A reference to the lower part of the number.*/
	const auto Lo() const
	{
		if (cg::Endian::little)
			return Num<const DemotedBaseType&>
			(*((DemotedBaseType*)&m_data));
		else
			return Num<const DemotedBaseType&>
			(*(((DemotedBaseType*)&m_data) + 1));
	}
	/**The internal data.*/
	StoreType m_data;
};


/**Create a Num in a wrapper.
\param n The number.
\return A number in the Num wrapper, which references the parameter `n`.*/
template<typename T>
auto MakeNum(T&& n)
{
	/*Will be true if T is deduced to be const& or T&&.*/
	const static bool IsConst
		/*Is Const& */
		= std::is_const<std::remove_reference_t<T>>::value
		/*Or T&& */
		|| std::is_rvalue_reference<T&&>::value;
	/*The base type of data when all the qualifiers are stripped away.*/
	using RawBaseType
		= std::remove_const_t<std::remove_reference_t<T>>;
	/*The Type that has been deduced to be Num Template parameter.*/
	using Type = std::conditional_t<
		IsConst,
		const T&,
		T&
	>;
	/*Return the Proper num object.*/
	return cg::Num<Type>(std::forward<T>(n));
}
/**Create a Num in a wrapper.
\param n The number.
\return A number in the Num wrapper, which references the parameter `n`.*/
template<typename T>
auto MakeNum(const Num<T>& n)
{
	return MakeNum(n.Get());
}
/**Create a Num in a wrapper.
\param n The number.
\return A number in the Num wrapper, which references the parameter `n`.*/
template<typename T>
auto MakeNum(Num<T>& n)
{
	return MakeNum(n.Get());
}

/**Create a Num in a wrapper.
\param n The number.
\return A number in the Num wrapper, which references the parameter `n`.*/
template<typename T>
auto MakeNumC(const T& n)
{
	return MakeNum(n);
}

/**Helper for making nums.*/
template<typename T, bool Ref>
using NumType = std::conditional_t<Ref,
	cg::Num<T&>,
	cg::Num<const T&> >;

/**Friender for the num.*/
class NumFriend
{
public:

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////SPLITTERS HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Get the hi part.
	\param n The num for which to get the part.
	\return the apropriate part of the num `n`.*/
	template<typename T>
	static auto Hi(Num<T>& n)
	{
		return n.Hi();
	}
	/**Get the hi part.
	\param n The num for which to get the part.
	\return the apropriate part of the num `n`.*/
	template<typename T>
	static auto Hi(const Num<T>& n)
	{
		return n.Hi();
	}
	/**Get the lo part.
	\param n The num for which to get the part.
	\return the apropriate part of the num `n`.*/
	template<typename T>
	static auto Lo(Num<T>& n)
	{
		return n.Lo();
	}
	/**Get the lo part.
	\param n The num for which to get the part.
	\return the apropriate part of the num `n`.*/
	template<typename T>
	static auto Lo(const Num<T>& n)
	{
		return n.Lo();
	}
};

/**Get the hi part.
\param n The num for which to get the part.
\return the apropriate part of the num `n`.*/
template<typename T>
auto Hi(Num<T>& n)
{
	return NumFriend::Hi(n);
}
/**Get the hi part.
\param n The num for which to get the part.
\return the apropriate part of the num `n`.*/
template<typename T>
auto Hi(const Num<T>& n)
{
	return NumFriend::Hi(n);
}
/**Get the lo part.
\param n The num for which to get the part.
\return the apropriate part of the num `n`.*/
template<typename T>
auto Lo(Num<T>& n)
{
	return NumFriend::Lo(n);
}
/**Get the lo part.
\param n The num for which to get the part.
\return the apropriate part of the num `n`.*/
template<typename T>
auto Lo(const Num<T>& n)
{
	return NumFriend::Lo(n);
}

template class Num<const uint64_t&>;

}
