#ifndef HAMMING_H
#define HAMMING_H

#include "Bitstream.hpp"
#include <cstdio>
//streamsize всегда в битах! Кроме hamming и rle всё в байтах.
namespace hamming {
	//r - количество битов контроля чётности на 1 блок
	_size_t get_encode_size (_size_t streamsize, _size_t r);
	_size_t get_decode_size (_size_t streamsize, _size_t r);
	void encode (_size_t streamsize, _size_t r, ibitstream& in, obitstream& out);
	void decode (_size_t streamsize, _size_t r, ibitstream& in, obitstream& out);
}

#endif
