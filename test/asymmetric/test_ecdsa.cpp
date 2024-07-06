#ifndef TEST_ASYMMETRIC_ECDSA_CPP_
#define TEST_ASYMMETRIC_ECDSA_CPP_

#include "test_ecdsa.hpp"

res_t test_ecdsa () {
	int _all = 0, bad = 0;
	for (auto& alg : ecdsa::available ()) {
		MESSAGE ("TESTING %s\n", alg->get_name ().c_str ());
		auto keys = alg->generate_keys ();
		long_number_t m = gen_randint<false> (alg->key_size () / 8);
		auto sign = keys.first->sign (m);
		if (!keys.second->check_sign (m, *sign)) {
			NOT_PASSED_ ("NOT PASSED!\n");
			++bad;
		}
		++_all;
	}
	/*const elliptic_curve* c = elliptic_curve::register_curve ("test", point {11570, 42257}, 114973, -3, 69424, 114467);
	const long_number_t d = 86109, n = c->get_n ();
	const point Q = c->mul_generator (d);
	MESSAGE ("Point Q at (%s, %s)\n", Q.get_x ().get (10).c_str (), Q.get_y ().get (10).c_str ());
	const long_number_t m = 1789679805;
	const long_number_t k = 84430;
	const point P = c->mul_generator (k);
	MESSAGE ("Point P at (%s, %s)\n", P.get_x ().get (10).c_str (), P.get_y ().get (10).c_str ());
	const long_number_t r = P.get_x () % n;
	const long_number_t s = (modular_invert (k, n) * (m + d * r)) % n;
	const ecdsa::public_key_t pub (c, Q);
	const ecdsa::sign_t sign (r, s);
	MESSAGE ("r = %s, s = %s, verify: %d\n", r.get (10).c_str (), s.get (10).c_str (), pub.check_sign (m, sign));*/
	return {_all - bad, _all};
}


#endif
