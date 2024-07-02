#ifndef ASYMMETRIC_EC_DIFFIE_HELLMAN_H_
#define ASYMMETRIC_EC_DIFFIE_HELLMAN_H_

#include "../base/crypt_base.hpp"
#include "../base/long_math.hpp"
#include "../ecc/elliptic_curve.hpp"
#include <vector>

class ec_diffie_hellman final : public asymmetric_exchange_t {
	const elliptic_curve* curve;
	ec_diffie_hellman (const elliptic_curve* _curve);
public:
	BLOCK_COPYING (ec_diffie_hellman);
	class message_t final : public asymmetric_exchange_t::message_t {
		point val;
	public:
		message_t (const point& _val) : val (_val) { }
		~message_t () { }
		point get_val () const {
			return val;
		}
		bool verify (asymmetric_exchange _alg) const override {
			return _alg == asymmetric_exchange::EC_DIFFIE_HELLMAN;
		}
		bool operator== (const asymmetric_exchange_t::message_t& other) const override final {
			if (!other.verify (asymmetric_exchange::EC_DIFFIE_HELLMAN)) {
				return false;
			}
			const message_t& _o = (const message_t&)other;
			return val == _o.get_val ();
		}
	};
	class public_key_t final : public asymmetric_exchange_t::public_key_t {
		point B;
		const elliptic_curve* curve;
	public:
		public_key_t (const point& _B, const elliptic_curve* _curve) : B (_B), curve (_curve) { }
		public_key_t (const public_key_t& other) : B (other.B), curve (other.curve) { }
		~public_key_t () { }
		bool verify (asymmetric_exchange _alg) const final override {
			return _alg == asymmetric_exchange::EC_DIFFIE_HELLMAN;
		}
		point get_b () const {
			return B;
		}
		const elliptic_curve* get_curve () const {
			return curve;
		}
	};
	class key_t final : public asymmetric_exchange_t::key_t {
		public_key_t val;
		long_number_t a;
		point A;
	public:
		key_t (const public_key_t& _val);
		key_t (const public_key_t& _val, const message_t& A);
		~key_t ();
		wrapper<asymmetric_exchange_t::message_t> gen_A () const final override;
		wrapper<asymmetric_exchange_t::message_t> gen_K (const asymmetric_exchange_t::message_t& other) const final override;
		wrapper<asymmetric_exchange_t::public_key_t> get_public () const final override;
	};
	~ec_diffie_hellman ();
	wrapper<asymmetric_exchange_t::key_t> gen_key (UNUSED _size_t keysize) const final override;
	wrapper<asymmetric_exchange_t::key_t> gen_default_key () const final override;
	wrapper<asymmetric_exchange_t::key_t> gen_other (const asymmetric_exchange_t::message_t& A, const asymmetric_exchange_t::public_key_t& pub) const final override;
	static const vector_view<wrapper<ec_diffie_hellman>> get_instances ();
	static void register_all ();
	_ADD(ec_dh);
};
_ADD_DEFINITION (ec_diffie_hellman);

#endif
