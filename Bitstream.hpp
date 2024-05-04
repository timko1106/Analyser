#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "StreamBase.hpp"

#ifdef __WINAPI_ENABLED__
#define _CRT_SECURE_NO_WARNINGS
#endif


using bit = bool;
struct pos : public base_pos {
	long long bitoffset;
	pos() : bitoffset(0) {
        base_pos::modifiers = BIT_OFFSET;
    }
	pos(long long byteoffset, long long bitoffset) : bitoffset(bitoffset) {
		base_pos::modifiers = BIT_OFFSET;
		base_pos::byteoffset = byteoffset;
	}
};
class ibitstream_base : public istream_base {
protected:
	int bitoffset;
public:
	ibitstream_base() : bitoffset(0) {}
	~ibitstream_base() { }
	virtual ibitstream_base& operator>>(bit& value) = 0;
	virtual istream_base& operator>>(char& c) = 0;
	virtual void read(char* data, size_t streamsize) = 0;
};
class ibitstream : public ibitstream_base, public stringstream_base {
public:
	ibitstream(const char* value, size_t buffer = FULL);
	~ibitstream() { }
	ibitstream_base& operator>>(bit& value);
	istream_base& operator>>(char& c);
	void read(char* data, size_t streamsize);
	void seekg(const base_pos& p);
	base_pos* tellg() const;
	bool eof() const { return stringstream_base::eof(); }
	size_t buff_size() const { return stringstream_base::buff_size(); }
	operator bool() const { return stringstream_base::operator bool(); }
};
class obitstream_base : public ostream_base {
protected:
	int bitoffset;
public:
	obitstream_base() : bitoffset(0) {}
	~obitstream_base() { }
	virtual obitstream_base& operator<<(bit value) = 0;
	virtual ostream_base& operator<< (char c) = 0;
	virtual void write(const char* data, size_t streamsize = FULL) = 0;
};
class obitstream : public obitstream_base, public stringstream_base {
public:
	obitstream(size_t buffer_size);
	~obitstream() { if (buff)delete[] buff; }
	obitstream_base& operator<<(bit value);
	ostream_base& operator<< (char c);
	void write(const char* data, size_t streamsize = FULL);
	bool eof() const { return stringstream_base::eof(); }
	size_t buff_size() const { return stringstream_base::buff_size(); }
	operator bool() const { return stringstream_base::operator bool(); }
	void seekg(const base_pos& p);
	base_pos* tellg() const;
};
template<typename Handle>
class obitstream_handled : public obitstream_base {
private:
	Handle handler;
public:
	obitstream_handled (const Handle& handler) : handler(handler) {}
	obitstream_base& operator<< (bit value) override { handler(value);return *this; }
	ostream_base& operator<< (char c) override {
		unsigned char _c = c;
		for (unsigned char i = 7; i >= 0; --i) {
			handler ((_c & (1 << i)) != 0);
		}
		return *this;
	}
	void write (const char* data, size_t streamsize = FULL) {
		if (streamsize == FULL)streamsize = strlen(data);
		while (streamsize--)(*this) << data[streamsize];
	}
	bool eof() const { return false; }
	size_t buff_size() const { return FULL; }
	operator bool() const { return true; }
	void seekg (const base_pos& p) { }
	base_pos* tellg() const { return nullptr; }
};

#endif
