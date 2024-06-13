#ifndef ASYMMETRIC_EL_GAMAL_CPP_
#define ASYMMETRIC_EL_GAMAL_CPP_

#include "../../include/analyser/asymmetric/el_gamal.hpp"

static long_number_t gen_session_key (const long_number_t& p, bool check = false) {
	_size_t bytes = (p.sizeinbase (2) + 7) / 8;
	long_number_t k = gen_randint<true> (bytes + 1);
	if (!check) {
		return k % (p - 3) + 2;
	}
	while (true) {
		k = k % (p - 3) + 2;
		if (gcd (k, p) == 1) {
			return k;
		}
		k = gen_randint<true> (bytes + 1);
	}
}

el_gamal::el_gamal () : asymmetric_t (asymmetric::EL_GAMAL, "ElGamal") { }

el_gamal::private_key_t::private_key_t (const long_number_t& x_, const long_number_t& p_, const long_number_t& g_) : x (x_), p (p_), g (g_) { }
el_gamal::private_key_t::~private_key_t () { }
long_number_t el_gamal::private_key_t::decrypt (const asymmetric_t::encrypted_t& c) const {
	if (!c.verify (asymmetric::EL_GAMAL)) {
		return INVALID;
	}
	const encrypted_t& enc = (const encrypted_t&)c;
	return (enc.get_v () * pow_prime_m (enc.get_u (), p - 1 - x, p)) % p;
}
wrapper<asymmetric_t::sign_t> el_gamal::private_key_t::sign (const long_number_t& m) const {
	while (true) {
		long_number_t k = gen_session_key (p, true);
		long_number_t r = pow_prime_m (g, k, p);
		long_number_t s = ((m - x * r) * modular_invert (k, p - 1)) % (p - 1);
		if (s != 0) {
			return wrapper<asymmetric_t::sign_t> (new sign_t {r, s});
		}
	}
}
el_gamal::public_key_t::public_key_t (const long_number_t& p_, const long_number_t& g_, const long_number_t& y_) : p (p_), g (g_), y (y_) { }
el_gamal::public_key_t::~public_key_t () { }
wrapper<asymmetric_t::encrypted_t> el_gamal::public_key_t::encrypt (const long_number_t& m) const {
	long_number_t k = gen_session_key (p, true);
	long_number_t u_ = pow_prime_m (g, k, p), v_ = pow_prime_m (y, k, p);
	while (v_ == 1 || u_ == 1 || (v_ * m) % p == 1) {
		k = gen_session_key (p, true);
		u_ = pow_prime_m (g, k, p);
		v_ = pow_prime_m (y, k, p);
	}
	//printf ("Key: %s\n", k.get (10).c_str ());
	return wrapper<asymmetric_t::encrypted_t>(new encrypted_t ((v_ * m) % p, u_ % p));
}
bool el_gamal::public_key_t::check_sign (const long_number_t& m, const asymmetric_t::sign_t& sign) const {
	if (!sign.verify (asymmetric::EL_GAMAL)) {
		return false;
	}
	const sign_t& _s = (const sign_t&)sign;
	long_number_t r = _s.get_r (), s = _s.get_s ();
	return (long_number_t (0) < r && r < p && long_number_t (0) < s && s < p - 1) && \
		(pow_prime_m (g, m, p) == (pow_prime_m (y, r, p) * pow_prime_m (r, s, p)) % p);
}
std::pair<wrapper<asymmetric_t::private_key_t>, wrapper<asymmetric_t::public_key_t>> el_gamal::generate_keys (_size_t bits) const {
	long_number_t p = gen_safe_prime (bits / 8 + 1);
	long_number_t x = gen_session_key (p);
	long_number_t g = get_primitive_root_prime (p);
	long_number_t y = pow_m (g, x, p);
	//printf ("x: %s\n", x.get (10).c_str ());
	return {wrapper<asymmetric_t::private_key_t>(new private_key_t (x, p, g)), wrapper<asymmetric_t::public_key_t>(new public_key_t (p, g, y))};
}

el_gamal::~el_gamal () { }

const el_gamal el_gamal::instance {};
long_number_t el_gamal::public_key_t::get_p () const {
	return p;
}
long_number_t el_gamal::public_key_t::get_g () const {
	return g;
}
long_number_t el_gamal::public_key_t::get_y () const {
	return y;
}

#endif
