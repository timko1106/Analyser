#ifndef BASE_STREAMS_STRINGSTREAM_CPP_
#define BASE_STREAMS_STRINGSTREAM_CPP_

#include "../../../include/analyser/base/stringstream.hpp"

stringstream_base::stringstream_base (char* buffer, _size_t buff_size, bool use) {
	if (buff_size == FULL) {
		if (buffer == nullptr) {
			buff_size = DEFAULT_SIZE;
		} else {
			buff_size = strlen (buffer) + INCLUDE_ZERO_TERMINATOR;
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
stringstream_base::stringstream_base (const char* buffer, _size_t buff_size) 
	: stringstream_base (const_cast<char*> (buffer), buff_size, false) {
}
stringstream_base::~stringstream_base() {
#if VERBOSE_DTORS
	printf ("~stringstream_base at %p ", (void*)this);
#endif
	destroy ();
}
void stringstream_base::destroy () {
	if (buff == nullptr) return;
	const _size_t size = buff_size ();
	if (size) {
		memset (buff, 0, size);
	}
#if VERBOSE_DTORS
	printf ("%p deleted\n", buff);
#endif
	delete[] buff;
	buff = curr = end = nullptr;
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
_size_t stringstream_base::used_size () const {
	return (_size_t)(curr - buff);
}
_size_t stringstream_base::free_size () const {
	return (_size_t)(end - curr);
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
wrapper<base_pos> stringstream_base::tellg () const {
	return wrapper<base_pos>(new base_pos{used_size ()});
}
void stringstream_base::seekg (const base_pos& to) {
	curr = buff + to.byteoffset;
	if (curr > end) {
		curr = end;
		return;
	}
	if (curr < buff) {
		curr = buff;
	}
}
void stringstream_base::resize (_size_t new_size) {
	if (buff == nullptr) {
		if (new_size == 0) {
			new_size = DEFAULT_SIZE;
		}
		buff = new char[new_size];
		memset (buff, 0, new_size);
		end = buff + new_size;
		seekg (base_pos {0, EMPTY});
		return;
	}
	if (buff_size () >= new_size) {
		return;
	}
	_size_t offset = used_size ();
	_size_t old_size = buff_size ();
	char* new_buff = new char[new_size];
	if (old_size) {
		memcpy (new_buff, buff, old_size);
	}
	memset (new_buff + old_size, 0, new_size - old_size);

	destroy ();

	buff = new_buff;
	end = buff + new_size;
	base_pos to (offset, EMPTY);
	seekg (to);
}
void stringstream_base::reset  () {
	base_pos p (0, EMPTY);
	seekg (p);
}
void stringstream_base::own (char* buffer, _size_t new_size) {
	destroy ();
	buff = buffer;
	end = buff + new_size;
	base_pos p (0, EMPTY);
	seekg (p);
}
void stringstream_base::reload (_size_t new_size) {
	if (new_size == 0) {
		new_size = DEFAULT_SIZE;
	}
	destroy ();

	buff = new char[new_size];
	end = buff + new_size;
	memset (buff, 0, new_size);
	base_pos p (0, EMPTY);
	seekg (p);
}
istringstream::istringstream (char* buffer, _size_t buffer_size, bool use) : stringstream_base (buffer, buffer_size, use) { }
istringstream::istringstream (const char* buffer, _size_t buffer_size) : stringstream_base (buffer, buffer_size) {}
istringstream::~istringstream () {
#if VERBOSE_DTORS
	printf ("~istringstream at %p\n", (void*)this);
#endif
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
_size_t istringstream::read (char* buffer, _size_t streamsize) {
	if (stringstream_base::eof ()) {
		return 0;
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
	return streamsize;
}

ostringstream::ostringstream (_size_t buff_size) : stringstream_base ((const char*)nullptr, buff_size ? buff_size : DEFAULT_SIZE) { }
ostringstream::~ostringstream () {
#if VERBOSE_DTORS
	printf ("~ostringstream at %p\n", (void*)this);
#endif
}
void ostringstream::write (const char* buffer, _size_t streamsize) {
	if (stringstream_base::eof ()) {
		resize (buff_size () * 2);
	}
	if (streamsize == FULL) {
		streamsize = strlen (buffer);
	}
	if (streamsize > free_size ()) {
		const _size_t _min = streamsize + used_size ();
		_size_t new_size = buff_size ();
		do {
			new_size *= 2;
		} while (_min > new_size);
		resize (new_size);
	}
	memcpy (curr, buffer, streamsize);
	curr += streamsize;
}
ostream_base& ostringstream::operator<< (char val) {
	if (stringstream_base::eof ()) {
		resize (buff_size () * 2);
	}
	*curr = val;
	++curr;
	return *this;
}

#endif
