#ifndef BITSTREAM_OUT_CPP
#define BITSTREAM_OUT_CPP

#include "Bitstream.hpp"

obitstream::obitstream(_size_t buffer_size) : obitstream_base (), ostringstream (buffer_size) { }

obitstream_base& obitstream::operator<<(bit value) {
	if (eof())return *this;
	if (value) {
		(*curr) |= ((unsigned char)1 << (7 - bitoffset));
	}
	else {
		(*curr) &= (unsigned char)(~((unsigned char)1 << (7 - bitoffset)));
	}
	++bitoffset;
	if (bitoffset == 8) {
		++curr;
		bitoffset = 0;
	}
	return *this;
}


void obitstream::seekg(const base_pos& p) {
	if (p.modifiers == EMPTY) {
		bitoffset = 0;
		stringstream_base::seekg (p);
		return;
	}
	const pos& _p = (const pos&)p;
	bitoffset = _p.bitoffset & 0b111;
	if (_p.byteoffset + buff >= end) {
		curr = end;
		return;
	}
	if (_p.byteoffset + curr < buff) {
		curr = buff;
		return;
	}
	curr = buff + _p.byteoffset;
}

base_pos* obitstream::tellg() const {
	return new pos(curr - buff, bitoffset);
}

#endif
