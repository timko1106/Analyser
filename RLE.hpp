#ifndef RLE_H
#define RLE_H

#include "Vars.hpp"
#include "Bitstream.hpp"
#include <cmath>

namespace rle {
	std::pair<char*, _size_t> decode(ibitstream_base& in, _size_t streamsize);
	_size_t encode(ibitstream_base& in, obitstream_base& out, _size_t streamsize);
}

#endif
