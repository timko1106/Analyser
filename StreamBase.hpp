#ifndef STREAM_BASE
#define STREAM_BASE

#include "Vars.hpp"

enum modifiers_t {
	EMPTY = 0x00,
	BIT_OFFSET = 0x01
};

struct base_pos {
	_size_t byteoffset;
	modifiers_t modifiers = EMPTY;
};

class iostream_base {
public:
	iostream_base() {}
	virtual ~iostream_base() { }
	virtual void seekg(const base_pos &p) = 0;
	virtual base_pos* tellg() const = 0;
	virtual bool eof() const = 0;
	operator bool() const { return !eof(); };
	virtual _size_t buff_size() const = 0;
};
class istream_base : public iostream_base {
public:
	virtual ~istream_base () { }
	virtual istream_base& operator>>(char& c) = 0;
	virtual void read(char* data, _size_t streamsize) = 0;
};
class ostream_base : public iostream_base {
public:
	virtual ~ostream_base () { }
	virtual ostream_base& operator<<(char c) = 0;
	virtual void write(const char* data, _size_t streamsize = FULL) = 0;
};
//stringstream НЕ ВЛАДЕЕТ, он копирует чужой буффер (не доверяем).
class stringstream_base : public iostream_base {
protected:
	char* buff = nullptr;
	char* end = nullptr;
	char* curr = nullptr;
public:
	stringstream_base (char* buffer, _size_t buff_size) {
		buff = buffer;
		end = buff + buff_size;
		curr = buff;
	}
	virtual ~stringstream_base() {
#if VERBOSE_DTORS
		printf ("~stringstream_base at %p ", this);
#endif
		if (buff) {
			delete[] buff;
#if VERBOSE_DTORS
			printf ("%p deleted", buff);
#endif
			buff = nullptr;
		}
#if VERBOSE_DTORS
		printf ("\n");
#endif
	}
	bool eof() const {
		return curr == end;
	}
	operator bool() const {
		return !eof();
	}
	_size_t buff_size() const {
		return end - buff;
	}
	const char* raw_view() {
		return buff;
	}
	char* eject () {
		char* tmp = buff;
		buff = nullptr;
		end = nullptr;
		curr = nullptr;
		return tmp;
	}
};

#endif
