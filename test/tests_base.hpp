#ifndef TESTS_BASE_H_
#define TESTS_BASE_H_

#include <cstring>
#include <vector>
#include "../include/analyser/base/vars.hpp"

using res_t = std::pair<int, int>;
using func_t = res_t(*)();

#define USE_COLOR 1

#if USE_COLOR 
#define RED "\033[91m"
#define GREEN "\033[92m"
#define RESTORE "\033[0m"
#else
#define RED "" 
#define GREEN "" 
#define RESTORE "" 
#endif

#define MESSAGE(format, ...) printf (format, __VA_ARGS__)
#define MESSAGE_(format) printf (format)
#define START(name) MESSAGE ("\n\nStarting %s tests\n", name)
#define NOT_PASSED(format, ...) MESSAGE (RED format RESTORE "\n", __VA_ARGS__)
#define OK(format, ...) MESSAGE (GREEN format RESTORE "\n", __VA_ARGS__)
#define NOT_PASSED_(format) MESSAGE_ (RED format RESTORE "\n")
#define OK_(format) MESSAGE_ (GREEN format RESTORE "\n")
#define BAD NOT_PASSED
#define BAD_ NOT_PASSED_
#define ADD_TEST(name, func_name) static const int __##func_name##__ = add (func_name, name)


#define DIFFIE_HELLMAN_TEST 1
#define EL_GAMAL_TEST 1
#define RSA_TEST 1
#define AES_TEST 1
#define HAMMING_TEST 1
#define XOR_TEST 1
#define RLE_TEST 1
#define BASE64_TEST 1


const _size_t KEY_SIZE_DH = 2048, KEY_SIZE_EL_GAMAL = 4096, KEY_SIZE_RSA = 4096;
const int TOTAL = 10;
//const char test_message[] = "HELLO!!!";
const char test_message[] = "HELLO!!! Ok, it's not too simple, but you can).\r\n\x0aReally simple.";
const _size_t TEST_SIZE = sizeof (test_message) - 1;

struct test_t {
	const func_t call;
	const char* name;
	test_t (func_t callable, const char* _name);
	res_t operator() ();
};

class view {
	std::vector<test_t>& data;
public:
	class iterator {
		test_t* ptr;
	public:
		iterator (test_t* _p);
		iterator& operator++ ();
		test_t operator* () const;
		bool operator== (const iterator&) const;
		bool operator != (const iterator&) const;
	};
	view (std::vector<test_t>& _data);
	const iterator begin () const;
	const iterator end () const;
};

view& get ();

int add (func_t callable, const char* name);

void raw_print (const char* buff, _size_t size);

void raw_bin (const char* buff, _size_t size);

#endif
