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


namespace cg {

/**Compiletime power operator.
*
*Get the power at compile time.
*
*\tparam num The number that is the base of the operation.
*\tparam power The power in which to raise the base.
*/
template<const std::size_t num, const std::size_t power>
class StaticPower {
private:
	/**\sa PowerEquals*/
	template<const std::size_t num, const std::size_t power>
	class Pow {
	public:
		static const std::size_t value = num* Pow<num, power - 1>::value;
	};
	/**\sa PowerEquals*/
	template<const std::size_t num>
	class Pow<num, 0> {
	public:
		static const std::size_t value = 1;
	};
public:
	static const std::size_t value = num* Pow<num, power - 1>::value;
};

template<const std::size_t num>
class StaticPower<num,0> {
public:
	static const std::size_t value = 1;
};

}
