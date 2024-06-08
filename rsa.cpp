#ifndef RSA_CPP
#define RSA_CPP

#include "rsa.hpp"
rsa::private_key_t::private_key_t (const long_number_t& p_, const long_number_t& q_, const long_number_t& d_, const long_number_t& n_) : p (p_), q (q_), d (d_), n (n_) {
	dp = d % (p - 1);
	dq = d % (q - 1);
	q_inv = modular_invert (q, p);
}
rsa::private_key_t::~private_key_t () { }
long_number_t rsa::private_key_t::decrypt (const asymmetric_t::encrypted_t& enc) const {
	if (!enc.verify (asymmetric::RSA)) {
		return INVALID;
	}
	long_number_t c = ((const rsa::encrypted_t&)enc).get_c ();
	//Через китайскую теорему об остатках
	long_number_t m1 = pow_prime_m (c, dp, p);
	long_number_t m2 = pow_prime_m (c, dq, q);
	long_number_t h = (q_inv * ((m1 - m2) % p)) % p;
	long_number_t m = (m2 + h * q) % n;
	return m;
}
long_number_t rsa::private_key_t::decrypt_long (const asymmetric_t::encrypted_t& enc) const {
	if (!enc.verify (asymmetric::RSA)) {
		return INVALID;
	}
	long_number_t c = ((const rsa::encrypted_t&)enc).get_c ();
	return pow_m (c, d, n);
}
wrapper<asymmetric_t::sign_t> rsa::private_key_t::sign (const long_number_t& m) const {
	rsa::encrypted_t _tmp (m);
	return wrapper<asymmetric_t::sign_t> (new sign_t (decrypt (_tmp)));
}
rsa::public_key_t::public_key_t (const long_number_t& n_, int e_) : n (n_), e (e_) { }
rsa::public_key_t::~public_key_t () { }
wrapper<asymmetric_t::encrypted_t> rsa::public_key_t::encrypt (const long_number_t& m) const {
	return wrapper<asymmetric_t::encrypted_t>{new encrypted_t (pow_m (m, e, n))};
}
long_number_t rsa::public_key_t::get_n () const {
	return n;
}
int rsa::public_key_t::get_e () const {
	return e;
}
bool rsa::public_key_t::check_sign (const long_number_t& m, const asymmetric_t::sign_t& s) const {
	if (!s.verify (asymmetric::RSA)) {
		return false;
	}
	long_number_t c = ((const rsa::sign_t&)s).get_c ();
	return pow_m (c, e, n) == m;
}
rsa::rsa () : asymmetric_t (asymmetric::RSA, "RSA") { }
rsa::~rsa () { }
std::pair<wrapper<asymmetric_t::private_key_t>, wrapper<asymmetric_t::public_key_t>> rsa::generate_keys (_size_t key_size) const {
	long_number_t p = gen_randprime (key_size / 16, true), q = gen_randprime (key_size / 16, true);
	while (q == p) {
		q = gen_randprime (key_size / 16, true);
	}
	long_number_t n = p * q;
	long_number_t phi = lcm ((p - 1), (q - 1));
	const int e = 65537;
	long_number_t d = modular_invert (e, phi);
	wrapper<asymmetric_t::public_key_t> pub (new public_key_t (n, e));
	wrapper<asymmetric_t::private_key_t> secured (new private_key_t (p, q, d, n));
	//printf ("Primes:\n%s\n%s\nn:%s\n", p.get ().c_str (), q.get ().c_str (), n.get ().c_str ());
	return {std::move (secured), std::move (pub)};
}

const rsa rsa::instance {};

#endif
