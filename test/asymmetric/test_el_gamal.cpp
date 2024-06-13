#ifndef TEST_ASYMMETRIC_EL_GAMAL_CPP_
#define TEST_ASYMMETRIC_EL_GAMAL_CPP_

#include "test_el_gamal.hpp"

res_t test_el_gamal () {
	auto keys = asymmetric_t::get ("ElGamal")->generate_keys (KEY_SIZE_EL_GAMAL);
	using pub_t = el_gamal::public_key_t;
	pub_t* pub = (pub_t*)keys.second.get ();
	MESSAGE_ ("Encryption tests:\n");
	int bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		long_number_t ex = gen_randint<false> (KEY_SIZE_EL_GAMAL / 8);
		auto enc = keys.second->encrypt (ex);
		long_number_t dec = keys.first->decrypt (*enc);
		if (ex != dec) {
			NOT_PASSED ("WRONG test %d, p: %s, g: %s, y: %s, encoding: %s, decoded: %s",\
					i, pub->get_p ().get ().c_str (), pub->get_g ().get ().c_str (),\
					pub->get_y ().get ().c_str (), ex.get ().c_str (),\
					dec.get ().c_str ());
			++bad;
		}
	}
	if (bad) {
		BAD ("Failed %d tests, total %d", bad, TOTAL);
	} else {
		OK ("Full ok %d tests", TOTAL);
	}
	int passed = TOTAL - bad;
	bad = 0;
	printf ("Signature tests\n");
	for (int i = 1; i <= TOTAL; ++i) {
		long_number_t m = gen_randint<false> (KEY_SIZE_EL_GAMAL / 8);
		auto signature = keys.first->sign (m);
		if (!keys.second->check_sign (m, *signature)) {
			NOT_PASSED ("WRONG test %d, p: %s, g: %s, y: %s, message: %s",\
					i, pub->get_p ().get ().c_str (), pub->get_g ().get ().c_str (),\
					pub->get_y ().get ().c_str (), m.get ().c_str ());
			++bad;
		}

	}
	if (bad) {
		BAD ("Failed %d tests, total %d", bad, TOTAL);
	} else {
		OK ("Passed %d tests", TOTAL);
	}
	passed += TOTAL - bad;
	const asymmetric_t* _el_gamal = asymmetric_t::get ("ElGamal");
	passed += (_el_gamal == &el_gamal::instance);
	MESSAGE_ ("Checking instance:\n");
	if (_el_gamal != &el_gamal::instance) {
		NOT_PASSED ("FAILED, got (%p), instance is (%p)", (const void*)_el_gamal, (const void*)&el_gamal::instance);
	} else {
		OK_ ("Passed");
	}
	return {passed, 2 * TOTAL + 1};
}

#endif
