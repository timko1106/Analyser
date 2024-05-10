#ifndef BITSTREAM_IN_CPP
#define BITSTREAM_IN_CPP

#include "Bitstream.hpp"

ibitstream::ibitstream (char* value, _size_t streamsize, bool own) : ibitstream_base (), istringstream (value, streamsize, own) { }

ibitstream::ibitstream(const char* value, _size_t streamsize) : ibitstream_base (), istringstream (const_cast<char*>(value), streamsize, false) { }
base_pos* ibitstream::tellg() const {
	return new pos(curr - buff, bitoffset);
}
void ibitstream::seekg(const base_pos& p){
	if (p.modifiers == EMPTY) {
		stringstream_base::seekg (p);
		bitoffset = 0;
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
