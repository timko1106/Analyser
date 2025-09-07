#ifndef BASE_BITSTREAM_H_
#define BASE_BITSTREAM_H_

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "stream_base.hpp"
#include "stringstream.hpp"

#ifdef __WINAPI_ENABLED__
#define _CRT_SECURE_NO_WARNINGS
#endif


using bit = bool;
struct pos : public base_pos {
	int bitoffset;
	pos () : bitoffset(0) {
		base_pos::modifiers = BIT_OFFSET;
	}
	pos(_size_t byteoff, int bitoff) : base_pos (byteoff, BIT_OFFSET), bitoffset(bitoff) { }
};
class ibitstream final : public istringstream {
	int bitoffset;
public:
	ibitstream (const char* value, _size_t buffer = FULL);
	ibitstream (char* value, _size_t buffer = FULL, bool own = false);
	~ibitstream () {
#if VERBOSE_DTORS
		printf ("~ibitstream at %p\n", (void*)this);
#endif
	}
	ibitstream& operator>>(bit& value);
	istream_base& operator>>(char& c) override {
		bitoffset = 0;
		return istringstream::operator>> (c);
	}
	_size_t read(char* data, _size_t streamsize) override {
		bitoffset = 0;
		return istringstream::read (data, streamsize);
	}
	void seekg(const base_pos& p) override;
	wrapper<base_pos> tellg() const override;
	/*bool eof() const override {
		return stringstream_base::eof();
	}
	_size_t buff_size() const override {
		return stringstream_base::buff_size();
	}
	_size_t used_size () const override {
		return stringstream_base::buff_size ();
	}
	_size_t free_size () const override {
		return stringstream_base::free_size ();
	}*/
	operator bool() const {
		return stringstream_base::operator bool();
	}
};
class obitstream : public ostringstream {
	int bitoffset;
public:
	obitstream (_size_t buffer_size);
	~obitstream () {
#if VERBOSE_DTORS
		printf ("~obitstream at %p\n", (void*)this);
#endif
	}
	obitstream& operator<<(bit value);
	ostream_base& operator<< (char c) override {
		bitoffset = 0;
		return ostringstream::operator<< (c);
	}
	void write(const char* data, _size_t streamsize = FULL) override {
		bitoffset = 0;
		ostringstream::write (data, streamsize);
	}
	/*bool eof() const override {
		return stringstream_base::eof();
	}
	_size_t buff_size() const override {
		return stringstream_base::buff_size();
	}
	_size_t used_size () const override {
		return stringstream_base::buff_size ();
	}
	_size_t free_size () const override {
		return stringstream_base::free_size ();
	}*/
	operator bool() const {
		return stringstream_base::operator bool();
	}
	void seekg(const base_pos& p) override;
	wrapper<base_pos> tellg() const override;
};

#endif
