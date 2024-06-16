#ifndef TEST_ASYMMETRICS_EXCHANGE_CPP_
#define TEST_ASYMMETRICS_EXCHANGE_CPP_

#include "test_asymmetrics_exchange.hpp"

res_t test_all_asymmetrics_exchange () {
	int _all = 0, bad = 0;
	for (auto& name : asymmetric_exchange_t::get_supported ()) {
		MESSAGE ("Testing %s\n", name.c_str ());
		const asymmetric_exchange_t* _alg = asymmetric_exchange_t::get (name);
		_size_t key_size = _alg->default_key_size ();
		auto key_a = _alg->gen_key (key_size);
		auto A = key_a->gen_A ();
		auto pub = key_a->get_public ();
		auto key_b = _alg->gen_other (*A, *pub);
		auto B = key_b->gen_A ();
		auto K_A = key_a->gen_K (*B);
		auto K_B = key_b->gen_K (*A);
		if (*K_A != *K_B) {
			++bad;
			NOT_PASSED_ ("NOT PASSED.");
		} else {
			OK_ ("PASSED");
		}
		++_all;
	}
	return {_all - bad, _all};
}

#endif 
