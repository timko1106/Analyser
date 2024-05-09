#ifndef BITSTREAM_IN_CPP
#define BITSTREAM_IN_CPP

#include "Bitstream.hpp"

ibitstream::ibitstream (char* value, _size_t streamsize, bool own) : ibitstream_base (), stringstream_base ((own ? value : new char[streamsize == FULL ? strlen (value) + 1 : streamsize]), streamsize == FULL ? strlen (value) + 1 : streamsize) {
	if (!own) {
		if (value != nullptr) {
			memcpy (buff, value, streamsize);
		} else {
			memset (buff, 0, streamsize);
		}
	}
}

ibitstream::ibitstream(const char* value, _size_t streamsize) : ibitstream_base (), stringstream_base (new char[streamsize == FULL ? strlen (value) + 1 : streamsize], streamsize == FULL ? strlen (value) : streamsize) {
	if (value != nullptr) {
		memcpy(buff, value, streamsize);
	}
	else {
		memset(buff, 0, streamsize);
	}
}
base_pos* ibitstream::tellg() const {
	return new pos(curr - buff, bitoffset);
}
void ibitstream::seekg(const base_pos& p){
	if (p.modifiers == EMPTY) {
		curr = buff + p.byteoffset;
		if (curr >= end) {
			curr = end;
		}
		if (curr < buff) {
			curr = buff;
		}
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

istream_base& ibitstream::operator>>(char& c) {
	if (eof()) {
		c = 0;
		return *this;
	}
	c = *curr;
	bitoffset = 0;
	++curr;
	return *this;
}

void ibitstream::read(char* data, _size_t streamsize) {
	if (eof()) {
		return;
	}
	if (streamsize > (_size_t)(end - curr)) {
		streamsize = end - curr;
	}
	memcpy(data, curr, streamsize);
	curr += streamsize;
	bitoffset = 0;
}

#endif
