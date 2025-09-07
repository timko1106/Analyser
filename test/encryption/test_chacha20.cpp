#ifndef TEST_ENCRYPTION_CHACHA20_CPP_
#define TEST_ENCRYPTION_CHACHA20_CPP_

#include "test_chacha20.hpp"

res_t test_chacha20 () {
	int _all = 0, bad = 0;
	{
		block_t key = random_generator::get_secure_randkey (chacha20::KEY_SIZE);
		istringstream in (test_message, TEST_SIZE);
		ostringstream out {};
		_size_t encoded_size = chacha20::instance.encrypt (in, out, key);
		in.own (out.eject (), encoded_size);
		out.reload (0);
		_size_t decrypted_size = encryption_t::get ("ChaCha20")->decrypt (in, out, key);
		out << '\00';
		if (raw_check_fail (out.raw_view (), TEST_SIZE) != 0) {
			++bad;
			NOT_PASSED ("WRONG, need: \"%s\", got: \"%s\"", test_message, out.raw_view ());
			MESSAGE ("%sneed: ", RED);
			raw_print (test_message, TEST_SIZE);
			MESSAGE_ ("\ngot: ");
			raw_print (out.raw_view (), decrypted_size);
			MESSAGE ("%s\n", RESTORE);
		}
	}
	++_all;
	{
		const char random_key_3[] = "\x15\xe3Lc\xf6Sv\x85\xde\xe9\xcc\xce\xe9\xf4\xaa\xfe\xfa\xf\xbd\x1\xbcZ+r\x00\x19i\xe8\x80_\xba\xffpl\xb6\xad\xab\xb7\x13h\xfe\xc0Wo";
		ifstream in ("chacha20_test1.png");
		ofstream out ("chacha20_test1_decoded.png");
		block_t key (random_key_3, sizeof (random_key_3) - 1);
		encryption_t::get ("ChaCha20")->decrypt (in, out, key);
		in.close ();
		out.close ();
		if (!check_if_equal ("chacha20_test1_decoded.png", "test1_need.png")) {
			NOT_PASSED_ ("Wrong test with file");
		}
		++_all;
	}
	return {_all - bad, _all};
}

#endif
