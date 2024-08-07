#ifndef BASE_STREAM_BASE_H_
#define BASE_STREAM_BASE_H_

#include "vars.hpp"

enum modifiers_t {
	EMPTY = 0x00,
	BIT_OFFSET = 0x01
};

struct base_pos {
	_size_t byteoffset;
	modifiers_t modifiers;
	base_pos (_size_t boffset = 0, modifiers_t mods = EMPTY) : byteoffset (boffset), modifiers (mods) { }
};

class iostream_base {
public:
	iostream_base() {}
	virtual ~iostream_base() { }
	virtual void seekg(const base_pos &p) = 0;
	virtual wrapper<base_pos> tellg() const = 0;
	virtual bool eof() const = 0;
	operator bool() const { return !eof(); };
	virtual _size_t buff_size() const = 0;
	virtual _size_t used_size () const = 0;
	virtual _size_t free_size () const = 0;
};
class istream_base : public iostream_base {
public:
	virtual ~istream_base () { }
	virtual istream_base& operator>>(char& c) = 0;
	virtual _size_t read(char* data, _size_t streamsize) = 0;
};
class ostream_base : public iostream_base {
public:
	virtual ~ostream_base () { }
	virtual ostream_base& operator<<(char c) = 0;
	virtual void write(const char* data, _size_t streamsize = FULL) = 0;
};


#endif
