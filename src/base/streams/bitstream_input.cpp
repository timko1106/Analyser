#ifndef BASE_STREAMS_BITSTREAM_IN_CPP_
#define BASE_STREAMS_BITSTREAM_IN_CPP_

#include "../../../include/analyser/base/bitstream.hpp"

ibitstream::ibitstream (char* value, _size_t streamsize, bool own) : ibitstream_base (), istringstream (value, streamsize, own) { }

ibitstream::ibitstream(const char* value, _size_t streamsize) : ibitstream_base (), istringstream (const_cast<char*>(value), streamsize, false) { }
wrapper<base_pos> ibitstream::tellg () const {
	return wrapper<base_pos>(new pos((_size_t)(curr - buff), bitoffset));
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

ibitstream_base& ibitstream::operator>>(bit& value) {
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
