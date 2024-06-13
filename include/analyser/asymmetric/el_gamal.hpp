#ifndef EL_GAMAL_H
#define EL_GAMAL_H

#include "long_math.hpp"
#include "crypt_base.hpp"

class el_gamal_ final : public asymmetric_t {
	el_gamal_ ();
public:
	class sign_t final : public asymmetric_t::sign_t {
		long_number_t r, s;
	public:
		sign_t (const long_number_t& _r, const long_number_t& _s) : r (_r), s (_s) { }
		~sign_t () { }
		bool verify (asymmetric _alg) const final {
			return _alg == asymmetric::EL_GAMAL;
		}
		long_number_t get_r () const {
			return r;
		}
		long_number_t get_s () const {
			return s;
		}
	};
	class encrypted_t final : public asymmetric_t::encrypted_t {
		long_number_t v, u;
	public:
		encrypted_t (const long_number_t& _v, const long_number_t& _u) : v (_v), u (_u) { }
		~encrypted_t () { }
		bool verify (asymmetric _alg) const final override {
			return _alg == asymmetric::EL_GAMAL;
		}
		long_number_t get_v () const {
			return v;
		}
		long_number_t get_u () const {
			return u;
		}
	};
	class public_key_t final : public asymmetric_t::public_key_t {
		long_number_t p, g, y;
	public:
		public_key_t (const long_number_t& _p, const long_number_t& _g, const long_number_t& _y);
		~public_key_t ();
		bool check_sign (const long_number_t& m, const asymmetric_t::sign_t& s) const final override;
		wrapper<asymmetric_t::encrypted_t> encrypt (const long_number_t& m) const final override;
		long_number_t get_p () const;
		long_number_t get_g () const;
		long_number_t get_y () const;
	};
	class private_key_t final : public asymmetric_t::private_key_t {
		long_number_t x, p, g;
	public:
		private_key_t (const long_number_t& _x, const long_number_t& _p, const long_number_t& _g);
		~private_key_t ();
		wrapper<asymmetric_t::sign_t> sign (const long_number_t& m) const final override;
		long_number_t decrypt (const asymmetric_t::encrypted_t& enc) const final override;
	};
	el_gamal_ (int) = delete;
	~el_gamal_ ();
	std::pair<wrapper<asymmetric_t::private_key_t>, wrapper<asymmetric_t::public_key_t>> generate_keys (_size_t key_size) const final;
	static const el_gamal_ instance;
};

#endif
