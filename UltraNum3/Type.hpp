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

#include <type_traits>
#include <cstdint>

namespace cg {


/**Get a Type that is exactly one demotion of T.
*
*The Type member is the numerical Type that is exactly one demotions of T.
*
*\tparam T The Type for which to get the demoted Type.
*/
template<typename T>
class DemoteType {
public:
	/*There is intentionaly no Type here.*/
	using Type = T;
};
/**\sa DemoteType*/
template<>
class DemoteType<uint64_t> {
public:
	/**The Type that has been demoted.*/
	using Type = uint32_t;

};
/**\sa DemoteType*/
template<>
class DemoteType<uint32_t> {
public:
	/**The Type that has been demoted.*/
	using Type = uint16_t;

};
/**\sa DemoteType*/
template<>
class DemoteType<uint16_t> {
public:
	/**The Type that has been demoted.*/
	using Type = uint8_t;

};

/**Get a Type that is exactly one promotion of T.
*
*The Type member is the numerical Type that is exactly one promotion of T. It
is the callers responsibility to ensure that there exists a Type that is larger
than T.
*
*\tparam T The Type for which to get the promoted Type.
*/
template<typename T>
class PromoteType {
public:
	/*There is intentionaly no Type here.*/
	using Type = T;
};
/**\sa PromoteType*/
template<> class PromoteType<uint32_t> {
public:
	/**The Type that has been promoted.*/
	using Type = uint64_t;

};
/**\sa PromoteType*/
template<> class PromoteType<uint16_t> {
public:
	/**The Type that has been promoted.*/
	using Type = uint32_t;

};
/**\sa PromoteType*/
template<> class PromoteType<uint8_t> {
public:
	/**The Type that has been promoted.*/
	using Type = uint16_t;

};
/**Determine the greater number in a static way. If they are equal it will
be that value.*/
template<std::size_t LHS, std::size_t RHS>
class Larger
{
public:
	/*The value of the computation.*/
	const static std::size_t value = LHS > RHS ? LHS : RHS;
};
/**Determine if LHS is greater than RHS*/
template<std::size_t LHS, std::size_t RHS>
class Great
{
public:
	/*The value of the computation.*/
	const static bool value = LHS > RHS ? true : false;
};
/**Determine if LHS is greater than RHS*/
template<std::size_t LHS, std::size_t RHS>
class GreatEq
{
public:
	/*The value of the computation.*/
	const static bool value = LHS >= RHS ? true : false;
};
/**Determine the lesser number in a static way. If they are equal it will
be that value.*/
template<std::size_t LHS, std::size_t RHS>
class Lesser
{
public:
	/*The value of the computation.*/
	const static std::size_t value = LHS < RHS ? LHS : RHS;
};
/**Determine if LHS is less than RHS*/
template<std::size_t LHS, std::size_t RHS>
class Less
{
public:
	/*The value of the computation.*/
	const static bool value = LHS < RHS ? true : false;
};
/**Determine if LHS is less than RHS*/
template<std::size_t LHS, std::size_t RHS>
class LessEq
{
public:
	/*The value of the computation.*/
	const static bool value = LHS <= RHS ? true : false;
};
/**Determine if LHS is the same as RHS*/
template<std::size_t LHS, std::size_t RHS>
class Equal
{
public:
	/*The value of the computation.*/
	const static bool value = LHS == RHS ? true : false;
};

template<std::size_t Dividend, std::size_t Divisor>
struct Ceil
{
	const static std::size_t M = Dividend % Divisor;
	const static std::size_t D = Dividend / Divisor;
	const static bool Do = M != 0;
	const static std::size_t value = Do ? D + 1 : D;
};

/**An interface to splitting types.*/
template<typename T>
class Splitable
{
public:
	/**Get the hi bits.
	\return A demoted type that is the hi bits only.*/
	virtual T Hi() = 0;
	/**Get the hi bits.
	\return A demoted type that is the hi bits only.*/
	virtual const T Hi() const = 0;
	/**Get the lo bits.
	\return A demoted type that is the lo bits only.*/
	virtual typename T Lo() = 0;
	/**Get the lo bits.
	\return A demoted type that is the lo bits only.*/
	virtual const T Lo() const = 0;
};

/**Get the max value of a Type.  A wrapper so that user types will work.
\tparam Type the Type of max to get.
\return A const expr that is the max value of this Type.*/
template<typename Type>
inline constexpr std::enable_if_t<std::is_fundamental<Type>::value, Type>
/*For FUNDAMENTAL*/
Max()
{
	return std::numeric_limits<Type>::max();
}
/**Get the max value of a Type.  A wrapper so that user types will work.
\tparam Type the Type of max to get.
\return A const expr that is the max value of this Type.*/
template<typename Type>
inline constexpr std::enable_if_t<!std::is_fundamental<Type>::value, Type>
/*For NON-FUNDAMENTAL*/
Max()
{
	return Type::Max();
}



}

