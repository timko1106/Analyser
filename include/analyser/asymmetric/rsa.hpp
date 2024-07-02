#ifndef ASYMMETRIC_RSA_H_
#define ASYMMETRIC_RSA_H_

#include "../base/long_math.hpp"
#include "../base/crypt_base.hpp"

class rsa final : public asymmetric_t {
	rsa ();
public:
	BLOCK_COPYING (rsa);
	class sign_t final : public asymmetric_t::sign_t {
		long_number_t c;
	public:
		sign_t (const long_number_t& _c) : c (_c) { }
		~sign_t () { }
		bool verify (asymmetric _alg) const final {
			return _alg == asymmetric::RSA;
		}
		long_number_t get_c () const {
			return c;
		}
	};
	class encrypted_t final : public asymmetric_t::encrypted_t {
		long_number_t c;
	public:
		encrypted_t (const long_number_t& _c) : c (_c) { }
		~encrypted_t () { }
		bool verify (asymmetric _alg) const final override {
			return _alg == asymmetric::RSA;
		}
		long_number_t get_c () const {
			return c;
		}
	};
	class public_key_t final : public asymmetric_t::public_key_t {
		long_number_t n;
		int e;
	public:
		public_key_t (const long_number_t& _n, int _e = 65537);
		~public_key_t ();
		bool check_sign (const long_number_t& m, const asymmetric_t::sign_t& s) const final override;
		wrapper<asymmetric_t::encrypted_t> encrypt (const long_number_t& m) const final override;
		long_number_t get_n () const;
		int get_e () const;
	};
	class private_key_t final : public asymmetric_t::private_key_t {
		long_number_t p, q, dp, dq, q_inv, d, n;
	public:
		private_key_t (const long_number_t& p, const long_number_t& q, const long_number_t& d, const long_number_t& n);
		~private_key_t ();
		wrapper<asymmetric_t::sign_t> sign (const long_number_t& m) const final override;
		long_number_t decrypt (const asymmetric_t::encrypted_t& enc) const final override;
		long_number_t decrypt_long (const asymmetric_t::encrypted_t& enc) const;
	};
	~rsa ();
	std::pair<wrapper<asymmetric_t::private_key_t>, wrapper<asymmetric_t::public_key_t>> generate_keys (_size_t key_size) const final;
	static const rsa instance;
};

#endif
