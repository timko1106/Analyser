#ifndef STRINGSTREAM_CPP
#define STRINGSTREAM_CPP

#include "StringStream.hpp"

stringstream_base::stringstream_base (char* buffer, _size_t buff_size, bool use) {
	if (buff_size == FULL) {
		if (buffer == nullptr) {
			buff_size = 1;
		} else {
			buff_size = strlen (buffer) + 1;
		}
	}
	if (use) {
		buff = buffer;
	} else {
		buff = new char[buff_size];
		if (buffer == nullptr) {
			memset (buff, 0, buff_size);
		} else {
			memcpy (buff, buffer, buff_size);
		}
	}
	end = buff + buff_size;
	curr = buff;
}
stringstream_base::stringstream_base (const char* buffer, _size_t buff_size) {
	if (buff_size == FULL) {
		if (buffer == nullptr) {
			buff_size = 1;
		} else {
			buff_size = strlen (buffer) + 1;
		}
	}
	buff = new char[buff_size];
	if (buffer == nullptr) {
		memset (buff, 0, buff_size);
	} else {
		memcpy (buff, buffer, buff_size);
	}
	end = buff + buff_size;
	curr = buff;
}
stringstream_base::~stringstream_base() {
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
bool stringstream_base::eof () const {
	return curr == end;
}
stringstream_base::operator bool() const {
	return !eof();
}
_size_t stringstream_base::buff_size() const {
	return (_size_t)(end - buff);
}
const char* stringstream_base::raw_view() {
	return buff;
}
char* stringstream_base::eject () {
	char* tmp = buff;
	buff = nullptr;
	end = nullptr;
	curr = nullptr;
	return tmp;
}
base_pos* stringstream_base::tellg () const {
	return new base_pos{(_size_t)(curr - buff)};
}
void stringstream_base::seekg (const base_pos& to) {
	if (to.byteoffset + curr > end) {
		curr = end;
		return;
	}
	if (to.byteoffset + curr < buff) {
		curr = buff;
		return;
	}
	curr = curr + to.byteoffset;
}
istringstream::istringstream (char* buffer, _size_t buffer_size, bool use) : stringstream_base (buffer, buffer_size, use) { }
istringstream::~istringstream () {
#if VERBOSE_DTORS
	printf ("~istringstream at %p\n", this);
#endif
	}
void istringstream::seekg(const base_pos &p) {
	stringstream_base::seekg (p);
}
base_pos* istringstream::tellg() const {
	return stringstream_base::tellg ();
}
bool istringstream::eof() const {
	return stringstream_base::eof ();
}
_size_t istringstream::buff_size() const {
	return stringstream_base::buff_size ();
}
istream_base& istringstream::operator>> (char& val) {
	if (stringstream_base::eof ()) {
		val = '\00';
		return *this;
	}
	val = *curr;
	++curr;
	return *this;
}
void istringstream::read (char* buffer, _size_t streamsize) {
	if (stringstream_base::eof ()) {
		return;
	}
	_size_t real = streamsize;
	if (streamsize > (_size_t)(end - curr)) {
		streamsize = (_size_t)(end - curr);
	}
	memcpy (buffer, curr, streamsize);
	curr += streamsize;
	if (real > streamsize) {
		memset (buffer + streamsize, 0, real - streamsize);
	}
}

ostringstream::ostringstream (size_t buff_size) : stringstream_base ((const char*)nullptr, buff_size) { }
ostringstream::~ostringstream () {
#if VERBOSE_DTORS
	printf ("~ostringstream at %p\n", this);
#endif
}
void ostringstream::seekg(const base_pos &p) {
	stringstream_base::seekg (p);
}
base_pos* ostringstream::tellg() const {
	return stringstream_base::tellg ();
}
bool ostringstream::eof() const {
	return stringstream_base::eof ();
}
_size_t ostringstream::buff_size() const {
	return stringstream_base::buff_size ();
}
ostream_base& ostringstream::operator<< (char val) {
	if (stringstream_base::eof ()) {
		return *this;
	}
	*curr = val;
	++curr;
	return *this;
}
void ostringstream::write (const char* buffer, _size_t streamsize) {
	if (stringstream_base::eof ()) {
		return;
	}
	if (streamsize == FULL) {
		streamsize = strlen (buffer);
	}
	if (streamsize > (_size_t)(end - curr)) {
		streamsize = (_size_t)(end - curr);
	}
	memcpy (curr, buffer, streamsize);
	curr += streamsize;
}

#endif
