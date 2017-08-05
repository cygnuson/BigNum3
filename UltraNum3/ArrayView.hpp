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

#if _DEBUG
#include <iostream>
#endif

namespace cg {

template<typename T>
class ArrayView
{
public:
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////STATICS HERE
	///////////////////////////////////////////////////////////////////////////

	/**Create a copy data.
	\param other The other array view.
	\return The deep copied array view.*/
	inline static ArrayView<T> Copy(const ArrayView<T>& other)
	{
		ArrayView<T> av(other.Size());
		std::memcpy(av.data(), other.data(), other.Size()*sizeof(T));
		return av;
	}
	/**Create a copy data.
	\param data The other data to copy.
	\param size The other data size.
	\return The deep copied array view.*/
	inline static ArrayView<T> Copy(const T* data, std::size_t size)
	{
		ArrayView<T> av(size);
		std::memcpy(av.data(), data, size*sizeof(T));
		return av;
	}
	///////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////CONSTRUCTORS HERE
	///////////////////////////////////////////////////////////////////////////

	/**Create the array view.
	\param data A pointer to the data.
	\param size The size of the data in elements.*/
	ArrayView(T* data = nullptr, std::size_t size = 0)
		:m_data(data), m_size(size) {};
	/**Create the array view.
	\param size The size of the data in elements.*/
	ArrayView(std::size_t size)
		:m_data((T*)std::malloc(size * sizeof(T))), m_size(size)
	{
		m_own = true;
	};
	/**Create the array view.
	\param size The size of the data in elements.
	\param init The value to initialize each element with.*/
	template<typename U>
	ArrayView(std::size_t size, U&& init)
		:m_data((T*)std::malloc(size * sizeof(T))), m_size(size)
	{
#if _DEBUG
		if (!SUPRESSDEBUG)
		std::cout << "\n~~~DEBUG~~~Allocated " << size * sizeof(T) 
			<< " bytes.\n";
#endif
		m_own = true;
		Fill(std::forward<U>(init));
	};
	/**Move ctor
	\param other The thing to move.*/
	ArrayView(ArrayView<T>&& other)
	{
		m_data = other.m_data;
		m_own = other.m_own;
		m_size = other.m_size;
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_own = false;
	}
	/**Copy ctor
	\param other The thing to copy.  The array will be referenced and data
	will not be deep copied.*/
	ArrayView(const ArrayView<T>& other)
	{
		m_data = other.m_data;
		m_size = other.m_size;
	}
	/**Destruct and if needed, destroy the data.*/
	~ArrayView()
	{
		if (m_own)
			Delete();
	}
	///////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////OPERATORS= HERE
	///////////////////////////////////////////////////////////////////////////

	/**Move assign
	\param other The thing to move.*/
	inline void operator=(ArrayView<T>&& other)
	{
		/*make sure to delete our current data if needed.*/
		if (m_own)
			Delete();

		m_data = other.m_data;
		m_own = other.m_own;
		m_size = other.m_size;
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_own = false;
	}
	/**Copy assign
	\param other The thing  to copy.  Data will not be copied, only the
	pointer.  This object will not be able to delete the pointer.*/
	inline void operator=(const ArrayView<T>& other)
	{
		m_data = other.m_data;
		m_size = other.m_size;
	}
	///////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////UTILS HERE
	///////////////////////////////////////////////////////////////////////////

	/**Fill the array with something.
	\param o The object to fill the array with.*/
	template<typename U>
	inline void Fill(U&& o)
	{
		auto sz = Size();
		for(std::size_t i = 0; i < sz; ++i)
		{
			new (m_data + i) T(std::forward<U>(o));
		}
	}
	/**Zero out the array.*/
	inline void ZeroOut()
	{
		std::memset(m_data, 0, m_size * sizeof(T));
	}
	/**Determine if the view is empty or not.
	\return True if the view is empty and invalid.*/
	inline bool Good() const
	{
		return !m_data;
	}
	/**Get the size of the data region.
	\return The amount of elements for the data.*/
	inline const std::size_t Size() const
	{
		return m_size;
	}
	/**Call delete on the array. Should only be used when the ArrayView is
	constructed with just a size.  Has no effect if the pointer was not
	allocated during construction.*/
	inline void Delete()
	{
		delete[] m_data; 
#if _DEBUG
		if(!SUPRESSDEBUG)
			std::cout << "\n~~~DEBUG~~~Freed " << m_size * sizeof(T)
			<< " bytes.\n";
#endif
		m_data = nullptr;
		m_size = 0;
	}
	/**Copy this data to another location.
	\param data The location to write to.
	\param size The max size of the destination.*/
	inline void CopyTo(char* data, std::size_t size)
	{
		auto sSize = size < m_size ? size : m_size;
		std::memmove(data, m_data, sSize * sizeof(T));
	}
	/**Copy this data (as string) to another location.  The copy will stop when
	the null term for strings is reached.
	\param data The location to write to.
	\param maxSize The max size of the destination.*/
	inline void CopyStr(char* data, std::size_t maxSize)
	{
		std::size_t i = 0;
		do {
			data[i] = m_data[i];
		} while (m_data[i++] != 0);
	}

	///////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////ACCESSORS HERE
	///////////////////////////////////////////////////////////////////////////

	/**Operator access.
	\param index The index to access.
	\return The element at the index.*/
	inline T& operator[](std::size_t index)
	{
		return m_data[index];
	}
	/**Operator access.
	\param index The index to access.
	\return The element at the index.*/
	inline const T& operator[](std::size_t index) const
	{
		if (!Good())
			throw std::runtime_error("The array is invalid.");
		return m_data[index];
	}
	/**Get a pointer to the data.
	\return A pointer to the data.*/
	inline T* data()
	{
		return m_data;
	}
	/**Get a offset pointer to the data.
	\return A pointer to the data.*/
	inline T* Offset(std::ptrdiff_t off)
	{
		return m_data + off;
	}
	/**Get a offset pointer to the data.
	\return A pointer to the data.*/
	inline const T* Offset(std::ptrdiff_t off) const
	{
		return m_data + off;
	}
	/**Get a pointer to the data.
	\return A pointer to the data.*/
	inline operator T* ()
	{
		return m_data;
	}
	/**Get a const pointer to the data.
	\return A pointer to the data.*/
	inline const T* data() const
	{
		return m_data;
	}
	/**Get a pointer to the data.
	\return A pointer to the data.*/
	inline operator const T* ()const
	{
		return m_data;
	}
	/**Get a Begin() iterator for other lists.
	\return A poitner to the first element.*/
	inline T* Begin()
	{
		return m_data;
	}
	/**Get a Begin() iterator for other lists.
	\return A poitner to the first element.*/
	inline const T* Begin() const
	{
		return m_data;
	}
	/**Get a one past the end iterator for otehr list compat.
	\return A pointer that is one past the end.*/
	inline const T* End() const
	{
		return m_data + m_size;
	}
	/**Get a one past the end iterator for otehr list compat.
	\return A pointer that is one past the end.*/
	inline T* End()
	{
		return m_data + m_size;
	}
private:
	/**Debugger flag*/
	const static bool SUPRESSDEBUG = true;
	/**A pointer to the data.*/
	T* m_data;
	/**The size of the data.*/
	std::size_t m_size;
	/**True if the data should be deleted when finished.*/
	bool m_own = false;

};

template class ArrayView<int>;

}