#ifndef TESTS_CPP
#define TESTS_CPP

#include "tests.hpp"

void tests_init () {
	std::remove ("xor_test1_decoded.png");
	std::remove ("xor_test2_decoded.txt");
	std::remove ("xor_test1.png");
	std::remove ("xor_test2.txt");
	{
		ifstream in ("xor_test2_need.txt");
		ofstream out ("xor_test2.txt");
		const char random_key_2[] = "\x18\x17\x0A\x18\x19\x5A";
		block_t key_2 (random_key_2, sizeof (random_key_2) - 1);
		xor_cipher::instance.encrypt (in, out, key_2);
		in.close ();
		out.close ();
	}
	{
		ifstream in ("xor_test1_need.png");
		ofstream out ("xor_test1.png");
		const char random_key_1[] = "\x99\x17\x04\xAA\x59\x4F\xF0\xAB";
		block_t key_1 (random_key_1, sizeof (random_key_1) - 1);
		encryption_t::get ("XOR-0")->encrypt (in, out, key_1);
		in.close ();
		out.close ();
	}
}

res_t test_all () {
#define PRINT_SUPPORTED(_name, _type) \
	OK_ ("Supported " _name ":"); \
	for (auto name : _type::get_supported ()) { \
		MESSAGE ("%s ", name.c_str ()); \
	} \
	MESSAGE_ ("\n");
	tests_init ();
	PRINT_SUPPORTED ("encodings", encoding_t);
	PRINT_SUPPORTED ("encryption", encryption_t);
	PRINT_SUPPORTED ("asymmetric", asymmetric_t);
	PRINT_SUPPORTED ("asymmetric key exchange", asymmetric_exchange_t);
	int all = 0, success = 0;
	for (test_t val : get ()) {
		START (val.name);
		res_t result = val ();
		if (result.first != result.second) {
			BAD ("Passed: %d, failed: %d, total: %d", result.first, result.second - result.first, result.second);
		} else {
			OK ("Result: Passed %d tests!", result.second);
		}
		++success;
		++all;
	}
	return {success, all};
}

#endif
