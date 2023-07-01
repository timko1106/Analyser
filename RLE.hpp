#ifndef RLE_H
#define RLE_H

#include "Vars.hpp"
#include "Bitstream.hpp"
#include <cmath>

namespace rle {
	std::pair<char*, size_t> decode(ibitstream_base& in, size_t streamsize);
	//void decode(ibitstream& in, obitstream_base& out, size_t streamsize = FULL);
	size_t encode(ibitstream_base& in, obitstream_base& out, size_t streamsize);
}

#endif