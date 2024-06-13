#ifndef BITSTREAM_H
#define BITSTREAM_H

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
class ibitstream_base : public istream_base {
protected:
	int bitoffset;
public:
	ibitstream_base () : bitoffset(0) {}
	virtual ~ibitstream_base() {
#if VERBOSE_DTORS
		printf ("~ibitstream_base at %p\n", (void*)this);
#endif
	}
	virtual ibitstream_base& operator>>(bit& value) = 0;
	virtual istream_base& operator>>(char& c) = 0;
	virtual _size_t read(char* data, _size_t streamsize) = 0;
};
class ibitstream : public ibitstream_base, public istringstream {
public:
	ibitstream (const char* value, _size_t buffer = FULL);
	ibitstream (char* value, _size_t buffer = FULL, bool own = false);
	~ibitstream () {
#if VERBOSE_DTORS
		printf ("~ibitstream at %p\n", (void*)this);
#endif
	}
	ibitstream_base& operator>>(bit& value) override;
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
	bool eof() const override {
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
	}
	operator bool() const {
		return stringstream_base::operator bool();
	}
};
class obitstream_base : public ostream_base {
protected:
	int bitoffset;
public:
	obitstream_base () : bitoffset(0) {}
	virtual ~obitstream_base () {
#if VERBOSE_DTORS
		printf ("~obitstream_base at %p\n", (void*)this);
#endif
	}
	virtual obitstream_base& operator<<(bit value) = 0;
	virtual ostream_base& operator<< (char c) = 0;
	virtual void write(const char* data, _size_t streamsize = FULL) = 0;
};
class obitstream : public obitstream_base, public ostringstream {
public:
	obitstream (_size_t buffer_size);
	~obitstream () {
#if VERBOSE_DTORS
		printf ("~obitstream at %p\n", (void*)this);
#endif
	}
	obitstream_base& operator<<(bit value) override;
	ostream_base& operator<< (char c) override {
		bitoffset = 0;
		return ostringstream::operator<< (c);
	}
	void write(const char* data, _size_t streamsize = FULL) override {
		bitoffset = 0;
		ostringstream::write (data, streamsize);
	}
	bool eof() const override {
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
	}
	operator bool() const {
		return stringstream_base::operator bool();
	}
	void seekg(const base_pos& p) override;
	wrapper<base_pos> tellg() const override;
};

#endif
