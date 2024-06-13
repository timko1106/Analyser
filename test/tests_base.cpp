#ifndef TESTS_BASE_CPP_
#define TESTS_BASE_CPP_

#include "tests_base.hpp"

test_t::test_t (func_t callable, const char* _name) : \
		call (callable), name (_name) { }
res_t test_t::operator() () {
	return call ();
}

view::iterator::iterator (test_t* _p) : ptr (_p) { }

view::iterator& view::iterator::operator++ () {
	++ptr;
	return *this;
}

test_t view::iterator::operator* () const {
	return *ptr;
}

bool view::iterator::operator== (const iterator& other) const {
	return ptr == other.ptr;
}

bool view::iterator::operator != (const iterator& other) const {
	return ptr != other.ptr;
}

std::vector<test_t>& get_used () {
	static std::vector<test_t> data{};
	return data;
}

view::view (std::vector<test_t>& _data) : data (_data) { }
const view::iterator view::begin () const {
	return iterator (data.data ());
}
const view::iterator view::end () const {
	return iterator (data.data () + data.size ());
}

view& get () {
	static view view {get_used ()};
	return view;
}

int add (func_t callable, const char* name) {
	get_used ().push_back (test_t{callable, name});
	return 0;
}

void raw_print (const char* buff, _size_t size) {
	for (_size_t i = 0; i < size; ++i) {
		MESSAGE ("%02x", (((unsigned)buff[i]) & 0xff));
	}
}

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
	((byte) & 0x80 ? '1' : '0'), \
	((byte) & 0x40 ? '1' : '0'), \
	((byte) & 0x20 ? '1' : '0'), \
	((byte) & 0x10 ? '1' : '0'), \
	((byte) & 0x08 ? '1' : '0'), \
	((byte) & 0x04 ? '1' : '0'), \
	((byte) & 0x02 ? '1' : '0'), \
	((byte) & 0x01 ? '1' : '0')


void raw_bin (const char* buff, _size_t size) {
	for (_size_t i = 0; i < size; ++i) {
		unsigned char byte = (unsigned char)buff[i];
		MESSAGE (BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY (byte));
	}
}
#undef BYTE_TO_BINARY
#undef BYTE_TO_BINARY_PATTERN

#endif
