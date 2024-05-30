#ifndef EL_GAMAL_CPP
#define EL_GAMAL_CPP

#include "ElGamal.hpp"

namespace el_gamal {
	private_key_t::private_key_t (const long_number_t& x, const long_number_t& p, const long_number_t& g) : x (x), p (p), g (g) { }
	long_number_t private_key_t::decode (const std::pair<long_number_t, long_number_t>& c) {
		return (c.second * pow_prime_m (c.first, p - 1 - x, p)) % p;
	}
	std::pair<long_number_t, long_number_t> private_key_t::sign (const long_number_t& m) {
		while (true) {
			long_number_t k = gen_session_key (p, true);
			long_number_t r = pow_prime_m (g, k, p);
			long_number_t s = ((m - x * r) * modular_invert (k, p - 1)) % (p - 1);
			if (s != 0) {
				return {r, s};
			}
		}
	}
	public_key_t::public_key_t (const long_number_t& p, const long_number_t& g, const long_number_t& y) : p (p), g (g), y (y) { }
	std::pair<long_number_t, long_number_t> public_key_t::encode (const long_number_t& m) {
		long_number_t k = gen_session_key (p, true);
		long_number_t first = pow_prime_m (g, k, p), second = pow_prime_m (y, k, p);
		while (first == 1 || second == 1) {
			k = gen_session_key (p, true);
			first = pow_prime_m (g, k, p);
			second = pow_prime_m (y, k, p);
		}
		//printf ("Key: %s\n", k.get (10).c_str ());
		return std::pair<long_number_t, long_number_t>(first, (second * m) % p);
	}
	bool public_key_t::check_sign (const std::pair<long_number_t, long_number_t>& c, const long_number_t& m) {
		return (long_number_t (0) < c.first && c.first < p && long_number_t (0) < c.second && c.second < p - 1) && \
			(pow_prime_m (g, m, p) == (pow_prime_m (y, c.first, p) * pow_prime_m (c.first, c.second, p)) % p);
	}
	long_number_t gen_session_key (const long_number_t& p, bool check) {
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
	std::pair<private_key_t, public_key_t> generate (_size_t bits) {
		long_number_t p = gen_safe_prime (bits / 8 + 1);
		long_number_t x = gen_session_key (p);
		long_number_t g = get_primitive_root_prime (p);
		long_number_t y = pow_m (g, x, p);
		//printf ("x: %s\n", x.get (10).c_str ());
		return std::pair<private_key_t, public_key_t>(private_key_t (x, p, g), public_key_t (p, g, y));
	}
};

#endif
