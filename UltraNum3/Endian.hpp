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

namespace cg {

/*
value is FALSE for little endian systems, and TRUE for big endian systems.
*/
class Endian {
public:
	/**The value `little` is true on little endian system and false on big
	endian systems.
	*/
	const static bool little;
	/**The value `big` is false on little endian system and true on big
	endian systems.
	*/
	const static bool big;
private:
	/**The thing to check against*/
	const static uint32_t __ENDIANNESS_CHECKER;
};

}
