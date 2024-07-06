#ifndef ASYMMETRIC_DSA_H_
#define ASYMMETRIC_DSA_H_

#include "../base/crypt_base.hpp"
#include "../base/long_math.hpp"
#include "../ecc/elliptic_curve.hpp"

class ecdsa final : public digit_signature_t {
	const elliptic_curve* curve;
	ecdsa (const elliptic_curve* _curve);
public:
	BLOCK_COPYING (ecdsa);
	class sign_t final : public digit_signature_t::sign_t {
		long_number_t r, s;
	public:
		sign_t (const long_number_t& _r, const long_number_t& _s) : r (_r), s (_s) { }
		~sign_t () { }
		bool verify (signature _alg) const final {
			return _alg == signature::ECDSA;
		}
		long_number_t get_r () const {
			return r;
		}
		long_number_t get_s () const {
			return s;
		}
	};
	class public_key_t final : public digit_signature_t::public_key_t {
		const elliptic_curve* curve;
		const point Q;
	public:
		public_key_t (const elliptic_curve* _curve, const point& _Q);
		~public_key_t ();
		bool check_sign (const long_number_t& m, const digit_signature_t::sign_t& s) const final override;
	};
	class private_key_t final : public digit_signature_t::private_key_t {
		const elliptic_curve* curve;
		const long_number_t d;
	public:
		private_key_t (const elliptic_curve* _curve, const long_number_t& _d);
		~private_key_t ();
		wrapper<digit_signature_t::sign_t> sign (const long_number_t& m) const final override;
	};
	~ecdsa ();
	std::pair<wrapper<digit_signature_t::private_key_t>, wrapper<digit_signature_t::public_key_t>> generate_keys () const final;
	static void register_all ();
	static vector_view<wrapper<ecdsa>> available ();
	_ADD (ecdsa);
};
_ADD_DEFINITION(ecdsa);


#endif
