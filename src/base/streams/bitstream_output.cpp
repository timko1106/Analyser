#ifndef BASE_STREAMS_BITSTREAM_OUT_CPP_
#define BASE_STREAMS_BITSTREAM_OUT_CPP_

#include "../../../include/analyser/base/bitstream.hpp"

obitstream::obitstream(_size_t buffer_size) : ostringstream (buffer_size), bitoffset (0) { }

obitstream& obitstream::operator<<(bit value) {
	if (eof ()) {
		resize (buff_size () * 2);
	}
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

wrapper<base_pos> obitstream::tellg() const {
	return wrapper<base_pos>(new pos (used_size (), bitoffset));
}

#endif
