#ifndef DIFFIE_HELLMAN_H
#define DIFFIE_HELLMAN_H

#include "vars.hpp"
#include "long_math.hpp"
#include "crypt_base.hpp"

class diffie_hellman final : public asymmetric_exchange_t {
	diffie_hellman ();
public:
	diffie_hellman (int) = delete;
	class message_t final : public asymmetric_exchange_t::message_t {
		long_number_t val;
	public:
		message_t (long_number_t _val) : val (_val) { }
		~message_t () { }
		long_number_t get_val () const {
			return val;
		}
		bool verify (asymmetric_exchange _alg) const override {
			return _alg == asymmetric_exchange::DIFFIE_HELLMAN;
		}
		bool operator== (const asymmetric_exchange_t::message_t& other) const override final {
			if (!other.verify (asymmetric_exchange::DIFFIE_HELLMAN)) {
				return false;
			}
			const message_t& _o = (const message_t&)other;
			return val == _o.get_val ();
		}
	};
	class public_key_t final : public asymmetric_exchange_t::public_key_t {
		long_number_t p, g;
	public:
		public_key_t (const long_number_t& _p, const long_number_t& _g) : p (_p), g (_g) { }
		~public_key_t () { }
		bool verify (asymmetric_exchange _alg) const final override {
			return _alg == asymmetric_exchange::DIFFIE_HELLMAN;
		}
		long_number_t get_p () const {
			return p;
		}
		long_number_t get_g () const {
			return g;
		}
	};
	class key_t final : public asymmetric_exchange_t::key_t {
		public_key_t val;
		long_number_t a, A;
	public:
		key_t (const public_key_t& _val);
		key_t (const public_key_t& _val, const message_t& A);
		~key_t ();
		wrapper<asymmetric_exchange_t::message_t> gen_A () const final override;
		wrapper<asymmetric_exchange_t::message_t> gen_K (const asymmetric_exchange_t::message_t& other) const final override;
		wrapper<asymmetric_exchange_t::public_key_t> get_public () const final override;
	};
	~diffie_hellman ();
	wrapper<asymmetric_exchange_t::key_t> gen_key (_size_t key_size) const final override;
	wrapper<asymmetric_exchange_t::key_t> gen_other (const asymmetric_exchange_t::message_t& A, const asymmetric_exchange_t::public_key_t& pub) const final override;
	static const diffie_hellman instance;
};

#endif
