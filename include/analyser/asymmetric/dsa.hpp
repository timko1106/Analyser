#ifndef ASYMMETRIC_DSA_H_
#define ASYMMETRIC_DSA_H_

#include "../base/crypt_base.hpp"
#include "../base/long_math.hpp"

class dsa final : public digit_signature_t {
	_size_t N, L;
	dsa (_size_t _N, _size_t _L);
public:
	BLOCK_COPYING (dsa);
	class sign_t final : public digit_signature_t::sign_t {
		long_number_t r, s;
	public:
		sign_t (const long_number_t& _r, const long_number_t& _s) : r (_r), s (_s) { }
		~sign_t () { }
		bool verify (signature _alg) const final {
			return _alg == signature::DSA;
		}
		long_number_t get_r () const {
			return r;
		}
		long_number_t get_s () const {
			return s;
		}
	};
	class public_key_t final : public digit_signature_t::public_key_t {
		long_number_t p, q, g, y;
	public:
		public_key_t (const long_number_t& _p, const long_number_t& _q, const long_number_t& _g, const long_number_t& _y);
		~public_key_t ();
		bool check_sign (const long_number_t& m, const digit_signature_t::sign_t& s) const final override;
	};
	class private_key_t final : public digit_signature_t::private_key_t {
		long_number_t p, g, q, x;
	public:
		private_key_t (const long_number_t& _p, const long_number_t& _g, const long_number_t& _q, const long_number_t& _x);
		~private_key_t ();
		wrapper<digit_signature_t::sign_t> sign (const long_number_t& m) const final override;
	};
	~dsa ();
	std::pair<wrapper<digit_signature_t::private_key_t>, wrapper<digit_signature_t::public_key_t>> generate_keys () const final;
	static void register_all ();
	static vector_view<wrapper<dsa>> available ();
	_ADD (dsa);
};
_ADD_DEFINITION(dsa);



#endif
