#ifndef FILESTREAM_CPP
#define FILESTREAM_CPP

#include "FileStream.hpp"

ifstream::ifstream (const char* const name) : fs (name, std::ios_base::in) {
	_size_t begin = fs.tellg ();
	fs.seekg (0, std::ios_base::end);
	_size_t end = fs.tellg ();
	streamsize = end - begin;
	fs.seekg (0, std::ios_base::beg);
}
ifstream::~ifstream () {
	fs.close ();
}
void ifstream::close () {
	fs.close ();
}
void ifstream::read (char* dest, _size_t count) {
	fs.read (dest, count);
}
istream_base& ifstream::operator>> (char& val) {
	fs.get (val);
	return *this;
}
bool ifstream::eof () const {
	return fs.eof ();
}
bool ifstream::is_open () const {
	return fs.is_open ();
}
base_pos* ifstream::tellg () const {
	base_pos* res = new base_pos{};
	res->byteoffset = const_cast<std::fstream&>(fs).tellg();
	res->modifiers = EMPTY;
	return res;
}
void ifstream::seekg (const base_pos& pos) {
	fs.seekg (pos.byteoffset);
}
_size_t ifstream::buff_size () const {
	return streamsize;
}

ofstream::ofstream (const char* const name) : fs (name, std::ios_base::out) { }
ofstream::~ofstream () {
	fs.close ();
}
void ofstream::close () {
	fs.close ();
}
void ofstream::write (const char* dest, _size_t count) {
	fs.write (dest, count);
}
ostream_base& ofstream::operator<< (char val) {
	fs << val;
	return *this;
}
bool ofstream::eof () const {
	return fs.eof ();
}
bool ofstream::is_open () const {
	return fs.is_open ();
}
base_pos* ofstream::tellg () const {
	base_pos* res = new base_pos{};
	res->byteoffset = const_cast<std::fstream&>(fs).tellg();
	res->modifiers = EMPTY;
	return res;
}
void ofstream::seekg (const base_pos& pos) {
	fs.seekg (pos.byteoffset);
}
_size_t ofstream::buff_size () const {
	return FULL;
}


#endif
