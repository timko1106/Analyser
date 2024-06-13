#ifndef TEST_ASYMMETRIC_RSA_CPP_
#define TEST_ASYMMETRIC_RSA_CPP_

#include "test_rsa.hpp"

res_t test_rsa () {
	auto keys = asymmetric_t::get ("RSA")->generate_keys (KEY_SIZE_RSA);
	int fast_bad = 0, long_bad = 0;
	MESSAGE_ ("Encryption tests:\n");
	using pub_t = rsa::public_key_t;
	pub_t* _p = (pub_t*)keys.second.get ();
	long_number_t _n = _p->get_n ();
	int _e = _p->get_e ();
	for (int i = 1; i <= TOTAL; ++i) {
		long_number_t ex = gen_randint<false> (KEY_SIZE_RSA / 8);
		auto enc = keys.second->encrypt (ex);
		long_number_t dec1 = keys.first->decrypt (*enc), dec2 = ((rsa::private_key_t*)(keys.first.get ()))->decrypt_long (*enc);
		if (ex != dec1) {
			NOT_PASSED ("WRONG fast test: %d, n: %s, e: %d, encoding: %s, decoded: %s", i, _n.get ().c_str (), _e, ex.get ().c_str (), dec1.get ().c_str ());
			++fast_bad;
		}
		if (ex != dec2) {
			NOT_PASSED ("WRONG long test: %d, n: %s, e: %d, encoding: %s, decoded: %s", i, _n.get ().c_str (), _e, ex.get ().c_str (), dec2.get ().c_str ());
		}
		fast_bad += (ex != dec1);
		long_bad += (ex != dec2);
	}
	MESSAGE ("Fast fails: %d, long fails: %d, total: %d\n", fast_bad, long_bad, TOTAL);
	long_number_t p = 9817, q = 9907, e = 65537, c = 36076319, d = modular_invert (e, lcm (p - 1, q - 1)), n = p * q;
	std::pair<rsa::private_key_t, rsa::public_key_t> test {rsa::private_key_t (p, q, d, n), rsa::public_key_t (n)};
	long_number_t decrypted = test.first.decrypt (rsa::encrypted_t (c));
	bool ok = decrypted == 41892906;
	std::string res = decrypted.get (10);
	if (ok) {
		OK ("Test: decoded: %s, OK", res.c_str ());
	} else {
		NOT_PASSED ("WRONG test: decoded: %s, need: %d", res.c_str (), 41892906);
	}
	MESSAGE_ ("Signature tests:\n");
	int sign_bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		long_number_t m = gen_randint<false> (KEY_SIZE_RSA / 8);
		auto sign = keys.first->sign (m);
		if (!keys.second->check_sign (m, *sign)) {
			NOT_PASSED ("WRONG test: %d,  n: %s, e: %d, signing: %s", i, _n.get ().c_str (), _e, m.get ().c_str ());
			++sign_bad;
		}
	}
	if (sign_bad) {
		NOT_PASSED ("Not passed: fails: %d, total: %d", sign_bad, TOTAL);
	} else {
		OK ("Signature tests passed, total: %d!", TOTAL);
	}
	return {3 * TOTAL - fast_bad - long_bad - sign_bad + ok, 3 * TOTAL + 1};
}

#endif
