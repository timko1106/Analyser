#ifndef EL_GAMAL_H
#define EL_GAMAL_H

#include "LongMath.hpp"

namespace el_gamal {
	class private_key_t {
		long_number_t x, p, g;
	public:
		private_key_t (const long_number_t& x, const long_number_t& p, const long_number_t& g);
		long_number_t decode (const std::pair<long_number_t, long_number_t>& c);
		std::pair<long_number_t, long_number_t> sign (const long_number_t& m);
	};
	class public_key_t {
		long_number_t p, g, y;
	public:
		public_key_t (const long_number_t& p, const long_number_t& g, const long_number_t& y);
		std::pair<long_number_t, long_number_t> encode (const long_number_t& m);
		bool check_sign (const std::pair<long_number_t, long_number_t>& c, const long_number_t& m);
	};
	long_number_t gen_session_key (const long_number_t& p, bool check = false);
	std::pair<private_key_t, public_key_t> generate (_size_t bits);
};

#endif
