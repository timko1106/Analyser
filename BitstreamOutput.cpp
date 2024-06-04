#ifndef BITSTREAM_OUT_CPP
#define BITSTREAM_OUT_CPP

#include "Bitstream.hpp"

obitstream::obitstream(_size_t buffer_size) : obitstream_base (), ostringstream (buffer_size) { }

obitstream_base& obitstream::operator<<(bit value) {
	if (eof())return *this;
	if (value) {
		(*curr) |= (char)((unsigned char)((unsigned char)1 << ((unsigned char)7 - (unsigned char)bitoffset)));
	}
	else {
		(*curr) &= (char)((unsigned char)(~(unsigned char)((unsigned char)1 << (7 - bitoffset))));
	}
	++bitoffset;
	if (bitoffset == 8) {
		++curr;
		bitoffset = 0;
	}
	return *this;
}


void obitstream::seekg(const base_pos& p) {
	stringstream_base::seekg (p);
	if (p.modifiers == EMPTY) {
		bitoffset = 0;
		return;
	}
	if (p.modifiers != modifiers_t::BIT_OFFSET) {
		return;
	}
	const pos* _p = (const pos*)&p;
	bitoffset = (_p->bitoffset) & 0b111;
}

base_pos* obitstream::tellg() const {
	return new pos ((_size_t)(curr - buff), bitoffset);
}

#endif
