#ifndef ENDIAN_HPP
#define ENDIAN_HPP

#include <cstdint>


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
	const static uint32_t __ENDIANNESS_CHECKER;
};

}

#endif //ENDIAN_HPP