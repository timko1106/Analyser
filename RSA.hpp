#ifndef RSA_H
#define RSA_H

#include "LongMath.hpp"

namespace rsa {
	class private_key_t {
		long_number_t p, q, dp, dq, q_inv, d, n;
	public:
		private_key_t (const long_number_t& p, const long_number_t& q, const long_number_t& d, const long_number_t& n);
		long_number_t decrypt_fast (const long_number_t& c);
		long_number_t decrypt_long (const long_number_t& c);
	};
	class public_key_t {
		long_number_t n;
		int e;
	public:
		public_key_t (const long_number_t& n, int e = 65537);
		long_number_t encrypt (const long_number_t& m);
	};
	std::pair<private_key_t, public_key_t> generate (_size_t bits = 2048);
}

#endif
