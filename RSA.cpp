#ifndef RSA_CPP
#define RSA_CPP

#include "RSA.hpp"

namespace rsa {
	private_key_t::private_key_t (const long_number_t& p, const long_number_t& q, const long_number_t& d, const long_number_t& n) : p (p), q (q), n (n), d (d) {
		dp = d % (p - 1);
		dq = d % (q - 1);
		q_inv = modular_invert (q, p);
	}
	long_number_t private_key_t::decrypt_fast (const long_number_t& c) const {
		//Через китайскую теорему об остатках
		long_number_t m1 = pow_prime_m (c, dp, p);
		long_number_t m2 = pow_prime_m (c, dq, q);
		long_number_t h = (q_inv * ((m1 - m2) % p)) % p;
		long_number_t m = (m2 + h * q) % n;
		return m;
	}
	long_number_t private_key_t::decrypt_long (const long_number_t& c) const {
		return pow_m (c, d, n);
	}
	public_key_t::public_key_t (const long_number_t& n, int e) : e (e), n (n) { }

	long_number_t public_key_t::encrypt (const long_number_t& m) const {
		return pow_m (m, e, n);
	}
	long_number_t public_key_t::get_n () const {
		return n;
	}
	int public_key_t::get_e () const {
		return e;
	}
	std::pair<private_key_t, public_key_t> generate (_size_t bits) {
		long_number_t p = gen_randprime (bits / 16, true), q = gen_randprime (bits / 16, true);
		while (q == p) {
			q = gen_randprime (bits / 16, true);
		}
		long_number_t n = p * q;
		long_number_t phi = lcm ((p - 1), (q - 1));
		const int e = 65537;
		long_number_t d = modular_invert (e, phi);
		public_key_t pub (n, e);
		private_key_t secured (p, q, d, n);
		printf ("Primes:\n%s\n%s\nn:%s\n", p.get ().c_str (), q.get ().c_str (), n.get ().c_str ());
		return std::pair<private_key_t, public_key_t>{secured, pub};
	}
};


#endif
