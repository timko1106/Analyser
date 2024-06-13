#ifndef TEST_ASYMMETRIC_DIFFIE_HELLMAN_CPP_
#define TEST_ASYMMETRIC_DIFFIE_HELLMAN_CPP_

#include "test_diffie_hellman.hpp"

res_t test_diffie_hellman () {
	int bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		auto key_A = diffie_hellman::instance.gen_key (KEY_SIZE_DH);
		auto A = key_A->gen_A ();
		auto alg_ = asymmetric_exchange_t::get ("DiffieHellman");
		auto key_B = alg_->gen_other (*A, *(key_A->get_public ()));
		auto B = key_B->gen_A ();
		auto K_A = key_A->gen_K (*B);
		auto K_B = key_B->gen_K (*A);
		auto pub = key_A->get_public ();
		if (*K_A != *K_B) {
			++bad;
			using pub_t = diffie_hellman::public_key_t;
			using msg_t = diffie_hellman::message_t;
			pub_t* real = (pub_t*)pub.get ();
			msg_t* k_a = (msg_t*)K_A.get (), *k_b = (msg_t*)K_B.get (), *a = (msg_t*)A.get (), *b = (msg_t*)B.get ();
			NOT_PASSED ("Wrong test %d with p=%s,g=%s,A=%s,B=%s: K_A=%s, K_B=%s", \
					i, real->get_p ().get ().c_str (), real->get_g ().get ().c_str ()\
					, a->get_val ().get ().c_str (), b->get_val ().get ().c_str (), \
					k_a->get_val ().get ().c_str (), k_b->get_val ().get ().c_str ());
		}
		bad += (*K_A != *K_B);
	}
	if (bad) {
		BAD ("Failed %d from %d", bad, TOTAL);
	} else {
		OK ("Passed %d tests", TOTAL);
	}
	return {TOTAL - bad, TOTAL};
}

#endif
