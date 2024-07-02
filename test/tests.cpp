#ifndef TESTS_CPP
#define TESTS_CPP

#include "tests.hpp"

void tests_init () {
	_RUN();
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
		ifstream in ("test1_need.png");
		ofstream out ("xor_test1.png");
		const char random_key_1[] = "\x99\x17\x04\xAA\x59\x4F\xF0\xAB";
		block_t key_1 (random_key_1, sizeof (random_key_1) - 1);
		encryption_t::get ("XOR-0")->encrypt (in, out, key_1);
		in.close ();
		out.close ();
	}
	{
		ifstream in ("test1_need.png");
		ofstream out ("chacha20_test1.png");
		const char random_key_3[] = "\x15\xe3Lc\xf6Sv\x85\xde\xe9\xcc\xce\xe9\xf4\xaa\xfe\xfa\xf\xbd\x1\xbcZ+r\x00\x19i\xe8\x80_\xba\xffpl\xb6\xad\xab\xb7\x13h\xfe\xc0Wo";
		block_t key_1 (random_key_3, sizeof (random_key_3) - 1);
		encryption_t::get ("ChaCha20")->encrypt (in, out, key_1);
		in.close ();
		out.close ();
	}
}

res_t test_cycle (bool order) {
	int _all = 0, bad = 0;
	for (test_t val : get ()) {
		if (val.order != order) {
			continue;
		}
		START (val.name);
		res_t result = val ();
		if (result.first != result.second) {
			BAD ("Passed: %d, failed: %d, total: %d", result.first, result.second - result.first, result.second);
			++bad;
		} else {
			OK ("Result: Passed %d tests!", result.second);
		}
		++_all;
	}
	return {_all - bad, _all};
}

res_t test_all () {
#define PRINT_SUPPORTED(_name, _type) \
	OK_ ("Supported " _name ":"); \
	for (auto it : _type::get_supported ()) { \
		MESSAGE ("%s ", it.key ().c_str ()); \
	} \
	MESSAGE_ ("\n");
	tests_init ();
	PRINT_SUPPORTED ("encodings", encoding_t);
	PRINT_SUPPORTED ("encryption", encryption_t);
	PRINT_SUPPORTED ("asymmetric", asymmetric_t);
	PRINT_SUPPORTED ("asymmetric key exchange", asymmetric_exchange_t);
#undef PRINT_SUPPORTED
	res_t res_1 = test_cycle (true);
	res_t res_2 = test_cycle (false);
	return {res_1.first + res_2.first, res_1.second + res_2.second};
}

#endif
