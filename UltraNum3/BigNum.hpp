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
	using RefSelf = BigNum<BasicStoreType&, Units>;
	/**A self reference type.*/
	using NonRefSelf = BigNum<const BasicStoreType&, Units>;
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
	/**Create with a premade list.
	\param l The list.*/
	BigNum(const cg::List<NumDataType, Units>& l)
		:m_data(std::move(l.Copy()))
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
	///////////////////////////////////////////////////////////OPERATORS HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Do a math operation.
	\param r The thing to operate on this with.
	\return A reference to this.*/
	Self& operator+=(const BasicStoreType& r)
	{
		auto n = cg::MakeNum(r);
		cg::AddNums(Begin(), Size(), &n, 1);
		return *this;
	}
	/**Do a math operation.
	\param r The thing to operate on this with.
	\return A reference to this.*/
	template<typename U, std::size_t S>
	Self& operator+=(const BigNum<U,S>& r)
	{
		cg::AddNums(Begin(), Size(), r.Begin(), r.Size());
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////ACCESSORS HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Direct access to the data.
	\param i A compatibility param. Must be 1.
	\return The data as a T&.*/
	NumDataType& Get(std::size_t i)
	{
		return m_data.Get(i);
	}
	/**Direct access to the data.
	\param i A compatibility param. Must be 1.
	\return The data as a T&.*/
	const NumDataType& Get(std::size_t i) const
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
	void Set(std::size_t i, const BasicStoreType& n)
	{
		m_data.Get(i).Get() = n;
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////UTILITIES HERE//
	///////////////////////////////////////////////////////////////////////////

	/**Get a reference to the internal data of this object.
	\return A modifiable reference to the data of this object.*/
	RefSelf GetReference()
	{
		List<Num<BasicStoreType&>, Units> l;
		auto beg = m_data.Begin();
		auto end = m_data.End();
		for (; beg != end; ++beg)
			l.PushBack(beg->GetReference());
		return RefSelf(std::move(l));
	}
	///**Get a Copy of the data.
	//\return A copy of data of this object.*/
	//NonRefSelf GetReference() const
	//{
	//	List<Num<const BasicStoreType&>, Units> l;
	//	auto beg = m_data.Begin();
	//	auto end = m_data.End();
	//	for (; beg != end; ++beg)
	//		l.PushBack(beg->GetReference());
	//	return NonRefSelf(std::move(l));
	//}
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
		while (m_data.Get(i).IsZero())
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
	/**Create a copy of this thing that cuts all reference.
	\return A copy of the object with copied data and no references.*/
	NonRefSelf HardCopy() const
	{
		List<Num<const BasicStoreType&>, Units> l;
		auto beg = m_data.Begin();
		auto end = m_data.End();
		for (; beg != end; ++beg)
			l.EmplaceBack(beg->Get());
		return NonRefSelf(std::move(l));
	}
	/**Create a copy of this thing..
	\return A copy of the object.*/
	Self Copy() const
	{
		List<NumDataType, Units> l;
		auto beg = m_data.Begin();
		auto end = m_data.End();
		for (; beg != end; ++beg)
			l.EmplaceBack(*beg);
		return Self(std::move(l));
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
			beg->MakeTwoComp();
	}
	/**Apply compliment to this numbe.*/
	void MakeComp()
	{
		auto beg = Begin();
		auto end = End();
		for (; beg != end; ++beg)
			beg->MakeComp();
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
private:
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

/**Make a big num. If all items are rvals, it will be non-ref. if all items
are lvalues it will be a ref number. If there is a mix, then it will be non
ref.
\param t The first number. All types of ts must be same as the type of this
param.
\param ts The rest of the numbers.*/
template<std::size_t S, typename T, typename...Ts>
auto MakeBigNum(T&& t, Ts&&...ts)
{
	return BigNum<const std::decay_t<T>&, sizeof...(Ts)+1>(std::forward<T>(t),
		std::forward<Ts>(ts)...);
}
template<std::size_t S, typename T>
auto MakeBigNum(const BigNum<T, S>& n)
{
	return n.HardCopy();
}

template class BigNum<uint16_t&, 2>;
template class BigNum<const uint16_t&, 2>;
template class BigNum<uint16_t&, 16>;
template class BigNum<const uint16_t&, 16>;

}