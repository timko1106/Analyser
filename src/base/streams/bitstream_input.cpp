#ifndef BASE_STREAMS_BITSTREAM_IN_CPP_
#define BASE_STREAMS_BITSTREAM_IN_CPP_

#include "../../../include/analyser/base/bitstream.hpp"

ibitstream::ibitstream (char* value, _size_t streamsize, bool own) : istringstream (value, streamsize, own), bitoffset (0) { }

ibitstream::ibitstream(const char* value, _size_t streamsize) : istringstream (value, streamsize), bitoffset (0) { }
wrapper<base_pos> ibitstream::tellg () const {
	return wrapper<base_pos>(new pos(used_size (), bitoffset));
}
void ibitstream::seekg(const base_pos& p){
	stringstream_base::seekg (p);
	if (p.modifiers == EMPTY) {
		bitoffset = 0;
		return;
	}
	if (p.modifiers != modifiers_t::BIT_OFFSET) {
		return;
	}
	const pos* p_ = (const pos*)&p;
	bitoffset = (p_->bitoffset) & 0x111b;
}

ibitstream& ibitstream::operator>>(bit& value) {
	if (eof()) {
		value = false;
		return *this;
	}
	value = (unsigned char)(*curr) & ((unsigned char)1 << (7 - bitoffset));
	++bitoffset;
	if (bitoffset == 8) {
		bitoffset = 0;
		++curr;
	}
	return *this;
}

#endif
