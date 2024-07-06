#ifndef TEST_ASYMMETRIC_SIGNATURES_CPP_
#define TEST_ASYMMETRIC_SIGNATURES_CPP_

#include "test_signatures.hpp"

res_t test_all_signatures () {
	int _all = 0, bad = 0;
	for (auto& it : digit_signature_t::get_supported ()) {
		const std::string& name = it.key ();
		const digit_signature_t* _alg = it.value ();
		MESSAGE ("Starting %s\n", name.c_str ());
		const digit_signature_t* alg = digit_signature_t::get (name);
		if (_alg != alg) {
			NOT_PASSED_ ("WRONG POINTER");
			++bad;
			++_all;
			continue;
		}
		_size_t key_size = _alg->key_size ();
		MESSAGE ("Key size: %llu\n", alg->key_size ());
		auto keys = alg->generate_keys ();
		//1: Encrypt/decrypt
		long_number_t _subscript = gen_randint<false> (key_size / 8);
		auto signature = keys.first->sign (_subscript);
		bool valid = keys.second->check_sign (_subscript, *signature);
		if (!valid) {
			NOT_PASSED ("Error in algorithm %s: subscripted: %s, signature NOT PASSED", name.c_str (), _subscript.get ().c_str ());
			++bad;
		}
		if (valid) {
			OK ("Passed all tests in algorithm %s", name.c_str ());
		}
		++_all;
	}
	return {_all - bad, _all};
}

#endif
