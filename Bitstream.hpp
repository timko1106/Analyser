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
	ibitstream_base () : bitoffset(0) {}
	virtual ~ibitstream_base() {
#if VERBOSE_DTORS
		printf ("~ibitstream_base at %p\n", this);
#endif
	}
	virtual ibitstream_base& operator>>(bit& value) = 0;
	virtual istream_base& operator>>(char& c) = 0;
	virtual void read(char* data, _size_t streamsize) = 0;
};
class ibitstream : public ibitstream_base, public stringstream_base {
public:
    ibitstream (const char* value, _size_t buffer = FULL);
	ibitstream (char* value, _size_t buffer = FULL, bool own = false);
	~ibitstream () {
#if VERBOSE_DTORS
		printf ("~ibitstream at %p\n", this);
#endif
	}
	ibitstream_base& operator>>(bit& value) override;
	istream_base& operator>>(char& c) override;
	void read(char* data, _size_t streamsize) override;
	void seekg(const base_pos& p) override;
	base_pos* tellg() const override;
	bool eof() const override {
		return stringstream_base::eof();
	}
	_size_t buff_size() const override{
		return stringstream_base::buff_size();
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
		printf ("~obitstream_base at %p\n", this);
#endif
	}
	virtual obitstream_base& operator<<(bit value) = 0;
	virtual ostream_base& operator<< (char c) = 0;
	virtual void write(const char* data, _size_t streamsize = FULL) = 0;
};
class obitstream : public obitstream_base, public stringstream_base {
public:
	obitstream (_size_t buffer_size);
	~obitstream () {
#if VERBOSE_DTORS
		printf ("~obitstream at %p\n", this);
#endif
	}
	obitstream_base& operator<<(bit value) override;
	ostream_base& operator<< (char c) override;
	void write(const char* data, _size_t streamsize = FULL) override;
	bool eof() const override {
		return stringstream_base::eof();
    }
	_size_t buff_size() const override {
		return stringstream_base::buff_size();
    }
	operator bool() const {
		return stringstream_base::operator bool();
    }
	void seekg(const base_pos& p) override;
	base_pos* tellg() const override;
};
template<typename Handle>
class obitstream_handled : public obitstream_base {
private:
	Handle handler;
public:
	obitstream_handled (const Handle& handler) : handler(handler) {}
	obitstream_base& operator<< (bit value) override {
		handler(value);
		return *this;
    }
	ostream_base& operator<< (char c) override {
		unsigned char _c = c;
		for (unsigned char i = 0; i < 8; --i) {
			handler ((_c & (1 << (7 - i))) != 0);
		}
		return *this;
	}
	void write (const char* data, _size_t streamsize = FULL) override {
		if (streamsize == FULL) {
			streamsize = strlen(data);
        }
		while (streamsize--) {
			(*this) << data[streamsize];
        }
	}
	bool eof() const override {
		return false;
    }
	_size_t buff_size() const override {
		return FULL;
    }
	operator bool() const {
		return true;
    }
    //ignore
	void seekg (const base_pos& p) override { }
	base_pos* tellg() const override {
		return nullptr;
    }
};

#endif
