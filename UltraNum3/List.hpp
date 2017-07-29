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
////////////////////////////////////////////////////STACK HERE/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/**The data holding portion of the List.
\tparam DataType The type of data to use.
\tparam Size, The size of the list on the stack, or ZERO to be heap-expanding.
*/
template<typename DataType, std::size_t SizeP>
class Storage
{
public:
	/**The type of data to use for the group.  All objects must be of the same
	type or both types must be bi-convertible.*/
	using T = typename DataType;
	/**The type of this object.*/
	using SelfType = typename Storage<T, SizeP>;
	/**The max size.*/
	const static std::size_t MaxSize = SizeP;
	/**default ctor*/
	Storage(std::size_t = 8) :m_cap(SizeP), m_size(0)
	{

	};
	/**Move ctor, to make it work with
	\param other The thing to move.*/
	Storage(SelfType&& other)
		:m_cap(std::move(other.m_cap)), m_size(std::move(other.m_size))
	{
		for (std::size_t i = 0; i < other.m_size; ++i)
			new (Addr() + i) DataType(std::move(other.Addr()[i]));
	};
	/**Create the int with initial values.

	If SizeP != 0:
	only the first SizeP values will be used.
	If amount of values < SizeP the rest will be set to 0.
	If SizeP == 0
	All values will be inserted to the storage.

	\param vals The values to insert.*/
	Storage(std::initializer_list<DataType>&& vals)
		:m_cap(SizeP)
	{
		std::size_t smaller = vals.size() < SizeP ? vals.size() : SizeP;
		for (std::size_t i = 0; i < smaller; ++i)
			new (Addr() + i) DataType(std::move(*(vals.begin() + i)));
		m_size = smaller;
	}
	/**Determine if another element can be inserted.
	\return True if an insert now would NOT throw an exception.*/
	bool CanInsert() const
	{
		if (m_size == m_cap)
			return false;
		return true;
	}
	/**Push an object to an index.
	\param i The place to put the object.
	\param o The thing to push.*/
	void Insert(std::size_t i, const T& o)
	{
		if (i > m_size)
			throw std::runtime_error("Index out of bounds.");
		if (m_size == m_cap)
			throw std::runtime_error("The list is full.");
		if (m_size != i)
			std::memmove(Addr() + i + 1, Addr() + i,
				sizeof(T)*(m_cap - m_size));
		new (Addr() + i)T(o);
		++m_size;
	}
	/**Push an object to an index.
	\param i The place to put the object.
	\param o The thing to push.*/
	void Insert(std::size_t i, T& o)
	{
		if (i > m_size)
			throw std::runtime_error("Index out of bounds.");
		if (m_size == m_cap)
			throw std::runtime_error("The list is full.");
		if (m_size != i)
			std::memmove(Addr() + i + 1, Addr() + i,
				sizeof(T)*(m_cap - m_size));
		new (Addr() + i)T(o);
		++m_size;
	}
	/**Push an object to an index.
	\param i The place to put the object.
	\param o The thing to push.*/
	void Insert(std::size_t i, T&& o)
	{
		if (i > m_size)
			throw std::runtime_error("Index out of bounds.");
		if (m_size == m_cap)
			throw std::runtime_error("The list is full.");
		if (m_size != i)
			std::memmove(Addr() + i + 1, Addr() + i,
				sizeof(T)*(m_size - i));
		new (Addr() + i)T(std::forward<T>(o));
		++m_size;
	}
	/**Emplace an object to an index.
	\param i The place to put the object.
	\param nums The args to send to the ctor of type T.*/
	template<typename...Ts>
	void Emplace(std::size_t i, Ts&&... nums)
	{
		if (i > m_size)
			throw std::runtime_error("Index out of bounds.");
		if (m_size == m_cap)
			throw std::runtime_error("The list is full.");
		if (m_size != i)
			std::memmove(Addr() + i + 1, Addr() + i,
				sizeof(T)*(m_size - i));
		new (Addr() + i)T(std::forward<Ts>(nums)...);
		++m_size;
	}
protected:
	/**Get the address of the data.
	\return The address of the data.*/
	DataType* Addr()
	{
		return (DataType*)m_data;
	}
	/**Get the address of the data.
	\return The address of the data.*/
	const DataType* Addr()const
	{
		return (const DataType*)m_data;
	}
	/**Stop copying*/
	Storage(const SelfType&) = delete;
	/**Stop copying*/
	void operator=(const SelfType&) = delete;
	/**The maximum capacity of the storage*/
	std::size_t m_cap;
	/**The size of used up slots*/
	std::size_t m_size;
private:
	/**The storage area.  Units of char so that the values are not initialized.
	*/
	char m_data[MaxSize * sizeof(T)];
};

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////HEAP HERE//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/**The data holding portion of the List.
\tparam DataType The type of data to use.
\tparam Size, The size of the list on the stack, or ZERO to be heap-expanding.
*/
template<typename DataType>
class Storage<DataType, 0>
{
public:
	/**The type of data to use for the group.  All objects must be of the same
	type or both types must be bi-convertible.*/
	using T = typename DataType;
	/**The type of this object.*/
	using SelfType = typename Storage<T, 0>;
	/**The amount to expand when re allocating.*/
	const static std::size_t ExpandAmount = 8;
	/**default ctor
	\param cap The initial capacity.*/
	Storage(std::size_t cap = 0) :m_cap(8), m_size(0)
	{
		if (cap != 0)
			m_data = (DataType*) std::malloc(m_cap* sizeof(DataType));
		else
			m_data = nullptr;
	};
	/**Move ctor
	\param other The thing to move.*/
	Storage(SelfType&& other)
		:m_cap(std::move(other.m_cap)), m_data(std::move(other.m_data)),
		m_size(std::move(other.m_size))
	{
		other.m_data = nullptr;
	};
	/**Create the int with initial values.

	If SizeP != 0:
	only the first SizeP values will be used.
	If amount of values < SizeP the rest will be set to 0.
	If SizeP == 0
	All values will be inserted to the storage.

	\param vals The values to insert.*/
	Storage(std::initializer_list<DataType>&& vals)
	{
		std::size_t sz = vals.size();
		ExpandTo(sz);
		for (std::size_t i = 0; i < sz; ++i)
			new (Addr() + i) DataType(std::move(*(vals.begin() + i)));
		m_size = sz;
		m_cap = sz;
	}
	/**Move op
	\param other The thing to move.*/
	void operator=(SelfType&& other)
	{
		m_data = other.m_data;
		other.m_data = nullptr;
		m_size = other.m_size;
		m_cap = other.m_cap;
	}
	/**Clean up the data.*/
	virtual ~Storage()
	{
		if (m_data)
			delete[] m_data;
	}
	/**Determine if another element can be inserted.
	\return True if an insert now would NOT throw an exception.*/
	bool CanInsert() const
	{
		return true;
	}
	/**Push an object to an index.
	\param i The place to put the object.
	\param o The thing to push.*/
	void Insert(std::size_t i, const T& o)
	{
		if (i > m_size)
			throw std::runtime_error("Index out of bounds.");
		if (m_size == m_cap)
			ExpandTo(m_cap + ExpandAmount);
		if (m_size != i)
			std::memmove(Addr() + i + 1, Addr() + i,
				sizeof(T)*(m_cap - m_size));
		new (Addr() + i)T(o);
		++m_size;
	}
	/**Push an object to an index.
	\param i The place to put the object.
	\param o The thing to push.*/
	void Insert(std::size_t i, T& o)
	{
		if (i > m_size)
			throw std::runtime_error("Index out of bounds.");
		if (m_size == m_cap)
			ExpandTo(m_cap + ExpandAmount);
		if (m_size != i)
			std::memmove(Addr() + i + 1, Addr() + i,
				sizeof(T)*(m_cap - m_size));
		new (Addr() + i)T(o);
		++m_size;
	}
	/**Push an object to an index.
	\param i The place to put the object.
	\param o The thing to push.*/
	void Insert(std::size_t i, T&& o)
	{
		if (i > m_size)
			throw std::runtime_error("Index out of bounds.");
		if (m_size == m_cap)
			ExpandTo(m_cap + ExpandAmount);
		if (m_size != i)
			std::memmove(Addr() + i + 1, Addr() + i,
				sizeof(T)*(m_size - i));
		new (Addr() + i)T(std::forward<T>(o));
		++m_size;
	}
	/**Emplace an object to an index.
	\param i The place to put the object.
	\param nums The args to send to the ctor of type T.*/
	template<typename...Ts>
	void Emplace(std::size_t i, Ts&&... nums)
	{
		if (i > m_size)
			throw std::runtime_error("Index out of bounds.");
		if (m_size == m_cap)
			ExpandTo(m_cap + ExpandAmount);
		if (m_size != i)
			std::memmove(Addr() + i + 1, Addr() + i,
				sizeof(T)*(m_size - i));
		new (Addr() + i)T(std::forward<Ts>(nums)...);
		++m_size;
	}
private:
	/**The storage area.*/
	DataType* m_data;
protected:
	/**Stop copying*/
	Storage(const SelfType&) = delete;
	/**Stop copying*/
	void operator=(const SelfType&) = delete;
	/**The maximum capacity of the storage*/
	std::size_t m_cap;
	/**The size of used up slots*/
	std::size_t m_size;
	///////////////////////////////////////////////////////////////////////////
	/**Expand the array to X amount of elements.
	\param amt The amount to hold. If m_cap is >=, nothing happens.*/
	void ExpandTo(std::size_t amt)
	{
		if (m_cap >= amt)
			return;
		if (!m_data)
		{
			/**Dont initialize...*/
			m_data = (T*) std::malloc(sizeof(T)*amt);
			return;
		}
		/**Dont initialize...*/
		T* nData = (T*) std::malloc(sizeof(T)*amt);
		std::memmove(nData, Addr(), sizeof(T) * m_size);
		m_cap = amt;

		delete m_data;
		m_data = nData;
	}
	/**Get the address of the data.
	\return The address of the data.*/
	DataType* Addr()
	{
		return m_data;
	}
	/**Get the address of the data.
	\return The address of the data.*/
	const DataType* Addr()const
	{
		return m_data;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////LIST HERE//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/**Base impl for the list.*/
template<typename DataType, std::size_t SizeP>
class List : public Storage<DataType, SizeP>
{
public:
	/**The type of data to use for the group.  All objects must be of the same
	type or both types must be bi-convertible.*/
	using T = typename DataType;
	/**The type of this object.*/
	using SelfType = typename List<T, SizeP>;
	/**Create the list.
	\param initCap The initial capacity to start with.  Not relevent for
	SizeP > 0.*/
	List(std::size_t initCap) : Storage(initCap) {};
	/**Create the list with cap of 8.*/
	List() : Storage(8) {};
	/**Move ctor
	\param other The thing to move.*/
	List(SelfType&& other)
		:Storage(std::move(other)) {};
	/**Create the int with initial values.

	If SizeP != 0:
	only the first SizeP values will be used.
	If amount of values < SizeP the rest will be set to 0.
	If SizeP == 0
	All values will be inserted to the storage.

	\param vals The values to insert.*/
	List(std::initializer_list<DataType>&& vals)
		:Storage(std::forward<std::initializer_list<DataType>>(vals))
	{

	}
	/**Create the object with a begin and end.
	\param beg The first poitner.
	\param end One-past-last pointer.*/
	List(DataType* beg, DataType* end)
	{
		std::size_t sz = end - beg;
		if (sz < 1)
			throw std::invalid_argument("The pointers are the same, or "
				"in reverse order.");
		for (std::size_t i = 0; i < sz; ++i)
			PushBack(*(beg + i));
	}
	/**Create the object with a begin and size.
	\param beg The first poitner.
	\param size The amount of units starting with `beg`.*/
	List(DataType* beg, std::size_t size)
	{
		if (size == 0)
			throw std::invalid_argument("The size is zero.");
		for (std::size_t i = 0; i < size; ++i)
			PushBack(*(beg + i));
	}
	/**Set all available space. This will fill memory that is allocated, but
	not marked as "used".
	\param x The thing to set the space too.*/
	void FillUnused(const DataType& x)
	{
		for (std::size_t i = m_size; i < m_cap; ++i)
			new (Begin() + i)T(x);
	}
	/**Push a value, and its `x` amount of increments. Ex: (a,3) will push
	a, a+1, a+2, to the list. There will be `x` amount of insertions.
	\param num The number.
	\param x The amount to insert with increments.*/
	void PushBackValueRange(DataType num, std::size_t x)
	{
		for (std::size_t i = 0; i < x; ++i)
			PushBack(num + i);
	}
	/**Get the size of the storage.
	\return The amount of elements.*/
	std::size_t Size() const
	{
		return m_size;
	}
	/**Push an object to the back of the list.
	\param o The object.*/
	void PushBack(const T& o)
	{
		Insert(m_size, o);
	}
	/**Push an object to the back of the list.
	\param o The object.*/
	void PushBack(T& o)
	{
		Insert(m_size, o);
	}
	/**Emplace an object to the back of the list.
	\param o The object.*/
	template<typename...Ts>
	void EmplaceBack(Ts&&... o)
	{
		Emplace(m_size, std::forward<Ts>(o)...);
	}
	/**Emplace an object to the back of the list.
	\param o The object.*/
	template<typename...Ts>
	void EmplaceFront(Ts&&... o)
	{
		Emplace(0, std::forward<Ts>(o)...);
	}
	/**Push an object to the back of the list.
	\param o The object.*/
	void PushBack(T&& o)
	{
		Insert(m_size, std::move(o));
	}
	/**Push an object to the front of the list.
	\param o The object.*/
	void PushFront(const T& o)
	{
		Insert(0, o);
	}
	/**Push an object to the front of the list.
	\param o The object.*/
	void PushFront(T&& o)
	{
		Insert(0, std::move(o));
	}
	/**Get an element.
	\param i The index to get.
	\return The object at i.*/
	T& Get(std::size_t i)
	{
		if (i > m_size)
			throw std::invalid_argument("The index is out of bounds.");
		return Addr()[i];
	}
	/**Get an element without any dereference layer.
	\param i The index to get.
	\return The object at i.*/
	const T& Get(std::size_t i) const
	{
		if (i >= m_size)
			throw std::invalid_argument("The index is out of bounds.");
		return Addr()[i];
	}
	/**Get an element.
	\param i The index to get.
	\return The object at i.*/
	T& operator[](std::size_t i)
	{
		if (i > m_size)
			throw std::invalid_argument("The index is out of bounds.");
		return Get(i);
	}
	/**Get an element.
	\param i The index to get.
	\return The object at i.*/
	const T& operator[](std::size_t i) const
	{
		if (i >= m_size)
			throw std::invalid_argument("The index is out of bounds.");
		return Get(i);
	}
	/**Get the begin iterator.
	\return An iterator to the front of the list.*/
	T* Begin()
	{
		return Addr();
	}
	/**Get the begin iterator.
	\return An iterator to the front of the list.*/
	const T* Begin() const
	{
		return Addr();
	}
	/**Get an iterator to the end+1 of the list.
	\return An iterator to End+1*/
	T* End()
	{
		return Addr() + m_size;
	}
	/**Get an iterator to the end+1 of the list.
	\return An iterator to End+1*/
	const T* End() const
	{
		return Addr() + m_size;
	}
	/**Erase a unit from the storage.
	\param i The index to erase.*/
	void Erase(std::size_t i)
	{
		Erase(i, 1);
	}
	/**Erase a unit from the storage.
	\param i The index to erase.*/
	void Pop(std::size_t i)
	{
		Erase(i);
	}
	/**Erase a unit from the storage.
	\param i The index to erase.
	\param s The amount to erase in elements.*/
	void Erase(std::size_t i, std::size_t s)
	{
		std::memmove(Addr() + i, Addr() + i + s,
			(m_size - (i + s)) * sizeof(T));
		m_size -= s;
	}
	/**Erase a unit from the storage.
	\param i The index to erase.
	\param s The amount to erase in elements.*/
	void Pop(std::size_t i, std::size_t s)
	{
		Erase(i, s);
	}
	/**Pop off the last element.*/
	void PopBack()
	{
		Erase(m_size - 1);
	}
	/**Pop off the last element.
	\param amt The amount to pop at the end.*/
	void PopBack(std::size_t amt)
	{
		Erase(m_size - amt, amt);
	}
	/**Pop the front element.*/
	void PopFront()
	{
		Erase(0);
	}
	/**Pop the front element.
	\param amt The amount to pop at the front */
	void PopFront(std::size_t amt)
	{
		Erase(0, amt);
	}
	/**Create a deep copy of this object.
	\return A deep copy of this object.*/
	SelfType Copy() const
	{
		SelfType copy;
		copy.m_cap = m_cap;
		copy.m_size = m_size;
		std::memcpy(copy.Addr(), Addr(), m_size * sizeof(T));
		return copy;
	}
	/**Determine the total cap of the list.
	\return The max amout for this list, or 0 for no maximum.*/
	std::size_t MaxSize() const
	{
		return SizeP;
	}
};

template class List<int, 0>;
template class List<int, 1>;

}