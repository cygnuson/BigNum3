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

#include "Helpers.hpp"

namespace cg {

/**Option codes for the rational numbrs.*/
enum class OptRational : int
{
	/**NONE option*/
	None,
	/**The options to auto simplify after math operations.*/
	AutoSimplify = 1,
};

/**A Rational number form.

\tparam DataType Should have/be the following:
.swap(DataType&)
Optomized for *= -1/1/0
Optomized for += 0
Optomized for == 0 and != 0
Move assign/Ctor
Copy assign/Ctor
overloaded for all operators.
*/
template<typename DataType = uint64_t>
class RationalImpl
{
public:
	/**The type of data to use for the group.  All objects must be of the same
	type or both types must be bi-convertible.*/
	using T = typename DataType;
	/**The type of this object.*/
	using SelfType = typename RationalImpl<T>;
	/**Create a rational from a double.
	\param num The number to create from.
	\param pre The precision to use.  This will be the amount of decimal
	places kept.*/
	static SelfType Make(double num, uint64_t pre)
	{
		double intPart = 0;
		double floatPart = modf(num, &intPart);
		pre = (T)pow(10, pre);
		floatPart *= pre;
		return SelfType{(T)floatPart + ((T)pre * (T)intPart),
			(T)pre};
	}
	/**Create the rational number.
	\param n The numerator
	\param d The denominator
	\param opt Any options to use as bitflags.*/
	RationalImpl(const T& n, const T& d, OptRational opt = OptRational::None)
	{
		Set(n, d);
		if (opt == OptRational::AutoSimplify)
			AutoSimp(true);
	}
	/**Create the rational with a 1 in the denominator.
	\param n The numerator.
	\param opt Any options to use as bitflags.*/
	RationalImpl(const T& n, OptRational opt = OptRational::None)
	{
		Set(n, 1);
		if (opt == OptRational::AutoSimplify)
			AutoSimp(true);
	}
	/**move ctor
	\param other The thing to set this to.*/
	RationalImpl(SelfType&& other)
		:m_num(std::move(other.m_num)), m_den(std::move(other.m_den)),
		m_opt(std::move(other.m_opt))
	{

	}
	/**copy ctor
	\param other The thing to set this to.*/
	RationalImpl(const SelfType& other) 
		: m_num(other.m_num), m_den(other.m_den), m_opt(other.m_opt)
	{

	}
	/**set equal to operator
	\param other The thing to set this to.*/
	void operator=(const SelfType& other)
	{
		m_num = other.m_num;
		m_den = other.m_den;
		m_opt = other.m_opt;
	}
	/**set equal to operator
	\param other The thing to set this to.*/
	void operator=(SelfType&& other)
	{
		m_num = std::move(other.m_num);
		m_den = std::move(other.m_den);
		m_opt = std::move(other.m_opt);
	}
	/**Get the multiplication inverse of the rational
	\return A copy of the rational number as its multiplication inverse.*/
	SelfType MInverse() const
	{
		return SelfType(m_den, m_num);
	}
	/**Make this into its multiplication inverse.
	\return A reference to this.*/
	SelfType& MakeMInverse()
	{
		std::swap(m_num, m_den);
		return *this;
	}
	/**Get reciprical (multiply inverse)
	\return The reciprical of the rational.*/
	SelfType Recipricol() const
	{
		return MInverse();
	}
	/**Make this into its multiplication inverse.
	\return A reference to this.*/
	SelfType& MakeRecipricol()
	{
		std::swap(m_num, m_den);
		return *this;
	}
	/**Get the addition inverse
	\return The addition inverse (the negative of this).*/
	SelfType AInverse() const
	{
		auto copy = *this;
		copy.m_num = -copy.m_num;
		return copy;
	}
	/**Make this into its additive inverse.
	\return A reference to this.*/
	SelfType& MakeAInverse()
	{
		m_num *= -1;
		return *this;
	}
	/**Get the addition inverse
	\return The addition inverse (the negative of this).*/
	SelfType Opposite() const
	{
		return AInverse();
	}

	/**Make this into its additive inverse.
	\return A reference to this.*/
	SelfType& MakeOpposite()
	{
		m_num *= -1;
		return *this;
	}
	SelfType& operator +=(const SelfType& other)
	{
		m_num = other.m_den*m_num;
		m_num += other.m_num*m_den;
		m_den *= other.m_den;
		if (AutoSimp())
			Simplify();
		return *this;
	}
	SelfType& operator -=(const SelfType& other)
	{
		return *this += other.Opposite();
	}
	SelfType& operator *=(const SelfType& other)
	{
#if _DEBUG
		std::cout << "\noperator *= called, will take bit extra time.";
#endif
		m_num *= other.m_num;
		m_den *= other.m_den;
		if (AutoSimp())
			Simplify();
		return *this;
	}
	SelfType& operator /=(const SelfType& other)
	{
		return *this *= other.Recipricol();
	}
	SelfType  operator + (const SelfType& other)const
	{
		auto copy = *this;
		copy += other;
		return copy;
	}
	SelfType  operator - (const SelfType& other)const
	{
		auto copy = *this;
		copy -= other;
		return copy;
	}
	SelfType  operator * (const SelfType& other)const
	{
		auto copy = *this;
		copy *= other;
		return copy;
	}
	SelfType  operator / (const SelfType& other)const
	{
		auto copy = *this;
		copy /= other;
		return copy;
	}
	SelfType& operator++()
	{
		m_num += 1;
		return *this;
	}
	SelfType  operator++(int)
	{
		auto copy = *this;
		++*this;
		return copy;
	}
	SelfType& operator--()
	{
		m_num -= 1;
		return *this;
	}
	SelfType  operator--(int)
	{
		auto copy = *this;
		--*this;
		return copy;
	}

	bool operator<(const SelfType& other) const
	{
#if _DEBUG
		std::cout << "\noperator< called, will take bit extra time.";
#endif
		return (m_num * other.m_den) < (m_den * other.m_num);
	}
	bool operator<=(const SelfType& other) const
	{
#if _DEBUG
		std::cout << "\noperator<= called, will take bit extra time.";
#endif
		return (m_num * other.m_den) <= (m_den * other.m_num);
	}
	bool operator>(const SelfType& other) const
	{
		return !(*this <= other);
	}
	bool operator>=(const SelfType& other) const
	{
		return !(*this < other);
	}
	bool operator==(const SelfType& other) const
	{
#if _DEBUG
		std::cout << "\noperator== called, will take bit extra time.";
#endif
		return (m_num * other.m_den) == (m_den * other.m_num);
	}
	bool operator!=(const SelfType& other) const
	{
		return !(*this == other);
	}

	SelfType operator-() const 
	{
		return Opposite();
	}
	/**Scale this rational number up. Its evaluated value will not change.
	\param val The value to scale by.*/
	void ScaleUp(const T& val)
	{
		*this *= SelfType(val, val);
	}
	/**Scale this rational number down. Its evaluated value will not change.
	\param val The value to be scaled down with.  The value must be a multiple
	of each value in the rational number. If its not, the function will return
	false and the rational will be unchanged.*/
	bool ScaleDown(const T& val)
	{
#if _DEBUG
		std::cout << "\nScaleDown called, will take much extra time.";
#endif
		bool numMult = m_num % val == 0;
		bool denMult = m_den % val == 0;
		if (!numMult || !denMult)
			return false;
		m_num /= val;
		m_den /= val;
		return true;
	}
	/**Enable the auto simplify option on this number.
	\param set True for auto simplify to be on, false to be off.*/
	void AutoSimp(bool set)
	{
		cg::SetFlag(m_opt, OptRational::AutoSimplify, set);
	}
	/**Determine if auto simp is on or off.
	\return True if auto Simp is ON, and false if it is OFF.*/
	bool AutoSimp() const
	{
		return cg::ReadFlag(m_opt, OptRational::AutoSimplify);
	}
	/**Try to simplify the rational.*/
	void Simplify()
	{
#if _DEBUG
		std::cout << "\nSimplify called, will take much extra time (b/c GCD).";
#endif
		if (m_num < 0 && m_den < 0)
		{
			m_num = -m_num;
			m_den = -m_den;
		}
		auto gcd = GCD(m_num, m_den);
		if (gcd != 1)
		{
			m_num /= gcd;
			m_den /= gcd;
		}
	}
	/**Get the decimal value of the rational.
	\return The decimal value of evaluating the rational.
	\tparam R The type to be evaluated into.*/
	template<typename R>
	R Eval()
	{
		return R(m_num) / m_den;
	}
protected:
	/**Set the parts.
	\param n The numerator
	\param d The denominator.*/
	void Set(const T& n, const T& d)
	{
		if (d == 0)
			throw std::invalid_argument("The denominator cannot be zero.");
		m_num = n;
		m_den = d;
	}
	/**The numerator*/
	T m_num;
	/**The denominator*/
	T m_den;
	/**Any options that are present.*/
	OptRational m_opt = OptRational::None;
};

using Rational64 = RationalImpl<int64_t>;
template class RationalImpl<int64_t>;

}