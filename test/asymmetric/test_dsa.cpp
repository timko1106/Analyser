#ifndef TEST_ASYMMETRIC_DSA_CPP_
#define TEST_ASYMMETRIC_DSA_CPP_

#include "test_dsa.hpp"

res_t test_dsa () {
	int _all = 0, bad = 0;
	for (auto& alg : dsa::available ()) {
		MESSAGE ("TESTING %s\n", alg->get_name ().c_str ());
		auto keys = alg->generate_keys ();
		MESSAGE ("Key size: %llu\n", alg->key_size ());
		long_number_t m = gen_randint<false> (alg->key_size () / 8);
		auto sign = keys.first->sign (m);
		if (!keys.second->check_sign (m, *sign)) {
			dsa::sign_t* ptr = (dsa::sign_t*)sign.get ();
			NOT_PASSED ("Signature is NOT valid! Subscript of %s is (%s, %s)", m.get ().c_str (), ptr->get_r ().get ().c_str (), ptr->get_s ().get ().c_str ());
			++bad;
		}
		++_all;
	}
	return {_all - bad, _all};
}



#endif 
