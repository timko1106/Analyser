#ifndef TEST_ENCRYPTIONS_CPP_
#define TEST_ENCRYPTIONS_CPP_

#include "test_encryptions.hpp"

res_t test_all_encryptions () {
	const _size_t MIN_KEY_SIZE = 15;
	int _all = 0, bad = 0;
	for (auto& it : encryption_t::get_supported ()) {
		if (it.key () == "XOR-1") {
			continue;
		}
		MESSAGE ("Testing %s\n", it.key ().c_str ());
		const encryption_t* _alg = encryption_t::get (it.key ());
		if (_alg != it.value ()) {
			NOT_PASSED_ ("WRONG POINTER");
			++bad;
			++_all;
			continue;
		}
		istringstream in (test_message);
		ostringstream out {};
		_size_t key_size;
		if (_alg->need_fixed_key_size ()) {
			key_size = _alg->key_size ();
		} else {
			key_size = random_generator::get () % TEST_SIZE;
			key_size = std::max (key_size, MIN_KEY_SIZE);
		}
		block_t key = random_generator::get_randkey (key_size);
		block_t precount = block_t::empty;
		if (_alg->need_precounting ()) {
			precount = _alg->precount (key);
		}
		_size_t encrypted = _alg->encrypt (in, out, key, precount);
		in.own (out.eject (), encrypted);
		out.reload (0);
		_size_t decrypted = _alg->decrypt (in, out, key, precount);
		if (strcmp (test_message, out.raw_view()) != 0) {
			++bad;
			NOT_PASSED_ ("FAILED");
			MESSAGE ("%sneed: ", RED);
			raw_print (test_message, TEST_SIZE);
			MESSAGE_ ("\ngot: ");
			raw_print (out.raw_view (), decrypted);
			MESSAGE ("%s\n", RESTORE);
		}
		++_all;
	}
	return {_all - bad, _all};
}

#endif 
