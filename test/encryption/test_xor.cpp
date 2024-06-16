#ifndef TEST_ENCRYPTION_XOR_CPP_
#define TEST_ENCRYPTION_XOR_CPP_

#include "test_xor.hpp"

res_t test_xor () {
	const char png_signature[] = "\x89\x50\x4E\x47\x0D\x0A\x1a\x0A";
	block_t signature (png_signature, sizeof (png_signature) - 1);
	const char random_key_1[] = "\x69\x17\x0A\x19\x33\xA5\x67";
	block_t key_1 (random_key_1, sizeof (random_key_1) - 1);
	const char random_key_2[] = "\x18\x17\x0A\x18\x19\x5A";
	block_t key_2 (random_key_2, sizeof (random_key_2) - 1);

	//Test1: simple text
	istringstream in (test_message);
	ostringstream out (TEST_SIZE + 1);
	(void)xor_cipher::instance.encrypt (in, out, key_1);
	in.own (out.eject (), TEST_SIZE);
	out.reload (TEST_SIZE + 1);
	_size_t decrypted_size = xor_cipher::instance.decrypt (in, out, key_1);
	bool _1_success = strcmp (out.raw_view(), test_message) == 0;
	if (!_1_success) {
		NOT_PASSED ("WRONG first test, need: \"%s\", got: \"%.*s\"", test_message, (int)decrypted_size, out.raw_view ());
	}

	//Test2: decode by key
	ifstream in2 ("xor_test2.txt");
	ofstream out2 ("xor_test2_decoded.txt");
	encryption_t::get ("XOR-0")->decrypt (in2, out2, key_2);
	in2.close ();
	out2.close ();
	bool _2_success = check_if_equal ("xor_test2_decoded.txt", "xor_test2_need.txt");
	if (!_2_success) {
		NOT_PASSED_ ("WRONG second test");
	}

	//Test3: decode by signature
	ifstream in3 ("xor_test1.png");
	ofstream out3 ("xor_test1_decoded.png");
	//xor_cipher::instance_signature.decrypt (in3, out3, signature);
	encryption_t::get ("XOR-1")->decrypt (in3, out3, signature);
	in3.close ();
	out3.close ();
	bool _3_success = check_if_equal ("xor_test1_decoded.png", "test1_need.png");
	if (!_3_success) {
		NOT_PASSED_ ("WRONG third test");
	}

	return {_1_success + _2_success + _3_success, 3};
}

#endif
