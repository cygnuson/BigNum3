
#include "Endian.hpp"

namespace cg {

const uint32_t Endian::__ENDIANNESS_CHECKER = 1;

const bool Endian::big
= *((uint8_t*)(&__ENDIANNESS_CHECKER)) == 0;

const bool Endian::little
= *((uint8_t*)(&__ENDIANNESS_CHECKER)) != 0;

}
