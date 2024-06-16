#ifndef TEST_ASYMMETRICS_CPP_
#define TEST_ASYMMETRICS_CPP_

#include "test_asymmetrics.hpp"

res_t test_all_asymmetrics () {
	int _all = 0, bad = 0;
	for (auto& name : asymmetric_t::get_supported ()) {
		MESSAGE ("Starting %s\n", name.c_str ());
		const asymmetric_t* alg = asymmetric_t::get (name);
		const _size_t key_size = alg->default_key_size ();//bytes
		auto keys = alg->generate_keys (key_size);
		//1: Encrypt/decrypt
		long_number_t num = gen_randint<false> (key_size / 8);
		auto enc = keys.second->encrypt (num);
		long_number_t dec = keys.first->decrypt (*enc);
		bool valid1 = num == dec;
		if (!valid1) {
			NOT_PASSED ("Error in algorithm %s: source: %s, decoded: %s", name.c_str (), num.get ().c_str (), dec.get ().c_str ());
			++bad;
		}
		long_number_t _subscript = gen_randint<false> (key_size / 8);
		auto signature = keys.first->sign (_subscript);
		bool valid2 = keys.second->check_sign (_subscript, *signature);
		if (!valid2) {
			NOT_PASSED ("Error in algorithm %s: subscripted: %s, signature NOT PASSED", name.c_str (), _subscript.get ().c_str ());
			++bad;
		}
		if (valid1 && valid2) {
			OK ("Passed all tests in algorithm %s", name.c_str ());
		}
		_all += 2;
	}
	return {_all - bad, _all};

}

#endif 
