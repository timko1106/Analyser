#ifndef DIFFIE_HELLMAN_CPP
#define DIFFIE_HELLMAN_CPP

#include "DiffieHellman.hpp"

namespace diffie_hellman {
	key_t::key_t (const long_number_t& p_, const long_number_t& g_) : p (p_), g (g_) {
		_size_t bytes = (p.sizeinbase (2) + 7) / 8;
		a = gen_randint<true>(bytes + 1) % (p - 2) + 2;
		while (pow_prime_m (g, a, p) == 1) {
			a = gen_randint<true>(bytes + 1) % (p - 2) + 2;
		}
	}
	key_t::key_t (const long_number_t& p_, const long_number_t& g_, const long_number_t& B) : p (p_), g (g_) {
		_size_t bytes = (p.sizeinbase (2) + 7) / 8;
		a = gen_randint<true>(bytes + 1) % (p - 2) + 2;
		while (pow_prime_m (g, a, p) == 1 || pow_prime_m (B, a, p) == 1) {
			a = gen_randint<true>(bytes + 1) % (p - 3) + 2;
		}
	}
	long_number_t key_t::gen_A () const {
		return pow_prime_m (g, a, p);
	}
	long_number_t key_t::gen_K (const long_number_t& B) const {
		return pow_prime_m (B, a, p);
	}
	long_number_t key_t::get_p () const {
		return p;
	}
	long_number_t key_t::get_g () const {
		return g;
	}
	key_t generate (_size_t bits) {
		long_number_t p = gen_safe_prime (bits / 8 + 1);
		long_number_t g = get_primitive_root_prime (p);
		return key_t (p, g);
	}
}

#endif
