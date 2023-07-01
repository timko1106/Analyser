#ifndef BITSTREAM_OUT_CPP
#define BITSTREAM_OUT_CPP

#include "Bitstream.hpp"

obitstream::obitstream(size_t buffer_size) {
	buff = new char[buffer_size];
	curr = buff;
	end = buff + buffer_size;
	bitoffset = 0;
	memset(buff, 0, buffer_size);
}

obitstream_base& obitstream::operator<<(bit value) {
	if (eof())return *this;
	if (value) {
		(*curr) |= (1 << (7 - bitoffset));
	}
	else {
		(*curr) &= ~(1 << (7 - bitoffset));
	}
	++bitoffset;
	if (bitoffset == 8) {
		++curr;
		bitoffset = 0;
	}
	return *this;
}

ostream_base& obitstream::operator<<(char c) {
	if (eof())return *this;
	*curr = c;
	++curr;
	bitoffset = 0;
	return *this;
}

void obitstream::write(const char* data, size_t streamsize) {
	if (eof())return;
	if (streamsize > end - curr) {
		streamsize = end - curr;
	}
	memcpy(curr, data, streamsize);
	bitoffset = 0;
	curr += streamsize;
}

void obitstream::seekg(const base_pos& p) {
	if (p.modifiers == EMPTY)return;
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