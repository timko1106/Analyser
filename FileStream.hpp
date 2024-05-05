#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "StreamBase.hpp"
#include <fstream>

class ifstream : public istream_base {
	std::fstream fs;
	_size_t streamsize;
public:
	ifstream (const char* const name) : fs (name, std::ios_base::in) {
		_size_t begin = fs.tellg ();
		fs.seekg (0, std::ios_base::end);
		_size_t end = fs.tellg ();
		streamsize = end - begin;
		fs.seekg (0, std::ios_base::beg);
	}
	~ifstream () {
		fs.close ();
	}
	void close () {
		fs.close ();
	}
	void read (char* dest, _size_t count) {
		fs.read (dest, count);
	}
	istream_base& operator>> (char& val) {
		fs.get (val);
		return *this;
	}
	bool eof () const {
		return fs.eof ();
	}
	bool is_open () const {
		return fs.is_open ();
	}
	base_pos* tellg () const {
		base_pos* res = new base_pos{};
		res->byteoffset = const_cast<std::fstream&>(fs).tellg();
		res->modifiers = EMPTY;
		return res;
	}
	void seekg (const base_pos& pos) {
		fs.seekg (pos.byteoffset);
	}
	_size_t buff_size () const {
		return streamsize;
	}
};

class ofstream : public ostream_base {
	std::fstream fs;
public:
	ofstream (const char* const name) : fs (name, std::ios_base::out) { }
	~ofstream () {
		fs.close ();
	}
	void close () {
		fs.close ();
	}
	void write (const char* dest, _size_t count) {
		fs.write (dest, count);
	}
	ostream_base& operator<< (char val) {
		fs << val;
		return *this;
	}
	bool eof () const {
		return fs.eof ();
	}
	bool is_open () const {
		return fs.is_open ();
	}
	base_pos* tellg () const {
		base_pos* res = new base_pos{};
		res->byteoffset = const_cast<std::fstream&>(fs).tellg();
		res->modifiers = EMPTY;
		return res;
	}
	void seekg (const base_pos& pos) {
		fs.seekg (pos.byteoffset);
	}
	_size_t buff_size () const {
		return FULL;
	}
};

#endif
