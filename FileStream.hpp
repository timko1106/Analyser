#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "StreamBase.hpp"
#include <fstream>

class ifstream : public istream_base {
	std::fstream fs;
	_size_t streamsize;
public:
	ifstream (const char* const name);
	~ifstream ();
	void close ();
	void read (char* dest, _size_t count);
	istream_base& operator>> (char& val);
	bool eof () const;
	bool is_open () const;
	base_pos* tellg () const;
	void seekg (const base_pos& pos);
	_size_t buff_size () const;
};

class ofstream : public ostream_base {
	std::fstream fs;
public:
	ofstream (const char* const name);
	~ofstream ();
	void close ();
	void write (const char* dest, _size_t count);
	ostream_base& operator<< (char val);
	bool eof () const;
	bool is_open () const;
	base_pos* tellg () const;
	void seekg (const base_pos& pos);
	_size_t buff_size () const;
};

#endif
