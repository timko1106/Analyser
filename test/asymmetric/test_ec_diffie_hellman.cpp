#ifndef ASYMMETRIC_TEST_EC_DIFFIE_HELLMAN_CPP_
#define ASYMMETRIC_TEST_EC_DIFFIE_HELLMAN_CPP_

#include "test_ec_diffie_hellman.hpp"

res_t test_ec_diffie_hellman () {
	int bad = 0, _all = 0;
	for (auto& ecdh : ec_diffie_hellman::get_instances ()) {
		MESSAGE ("TESTING %s\n", ecdh->get_name ().c_str ());
		auto key_A = ecdh->gen_default_key ();
		auto A = key_A->gen_A ();
		auto alg_ = asymmetric_exchange_t::get (ecdh->get_name ());
		auto key_B = alg_->gen_other (*A, *(key_A->get_public ()));
		auto B = key_B->gen_A ();
		auto K_A = key_A->gen_K (*B);
		auto K_B = key_B->gen_K (*A);
		auto pub = key_A->get_public ();
		if (*K_A != *K_B) {
			++bad;
			using pub_t = ec_diffie_hellman::public_key_t;
			using msg_t = ec_diffie_hellman::message_t;
			UNUSED pub_t* real = (pub_t*)pub.get ();
			UNUSED msg_t* k_a = (msg_t*)K_A.get (), *k_b = (msg_t*)K_B.get (), *a = (msg_t*)A.get (), *b = (msg_t*)B.get ();
			NOT_PASSED_ ("Wrong test");
		}
		bad += (*K_A != *K_B);
		++_all;
	}
	if (bad) {
		BAD ("Failed %d from %d", bad, _all);
	} else {
		OK ("Passed %d tests", _all);
	}
	return {_all - bad, _all};
}

#endif
