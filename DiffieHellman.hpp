#ifndef DIFFIE_HELLMAN_H
#define DIFFIE_HELLMAN_H

#include "LongMath.hpp"

namespace diffie_hellman {
	class key_t {
		long_number_t p, g, a;
	public:
		key_t (const long_number_t& p, const long_number_t& g);
		key_t (const long_number_t& p, const long_number_t& g, const long_number_t& B);
		long_number_t gen_A () const;
		long_number_t gen_K (const long_number_t& B) const;
		long_number_t get_p () const;
		long_number_t get_g () const;
	};
	key_t generate (_size_t bits);
};

#endif
