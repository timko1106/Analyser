#ifndef BASE_FILESTREAM_H_
#define BASE_FILESTREAM_H_

#include "stream_base.hpp"
#include <fstream>
#include <cstring>

class ifstream : public istream_base {
	std::fstream fs;
	_size_t streamsize;
	std::streamoff beg;
public:
	ifstream (const char* const name);
	~ifstream ();
	void close ();
	_size_t read (char* dest, _size_t count);
	istream_base& operator>> (char& val);
	bool eof () const;
	bool is_open () const;
	wrapper<base_pos> tellg () const;
	void seekg (const base_pos& pos);
	_size_t buff_size () const;
	_size_t used_size () const;
	_size_t free_size () const;
};

class ofstream : public ostream_base {
	std::fstream fs;
	std::streamoff beg;
public:
	ofstream (const char* const name);
	~ofstream ();
	void close ();
	void write (const char* dest, _size_t count);
	ostream_base& operator<< (char val);
	bool eof () const;
	bool is_open () const;
	wrapper<base_pos> tellg () const;
	void seekg (const base_pos& pos);
	_size_t buff_size () const;
	_size_t used_size () const;
	_size_t free_size () const;
};

//Files are equal?
bool check_if_equal (const char* a, const char* b);

#endif
