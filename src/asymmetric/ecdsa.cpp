#ifndef ASYMMETRIC_ECDSA_CPP_
#define ASYMMETRIC_ECDSA_CPP_

#include "../../include/analyser/asymmetric/ecdsa.hpp"

ecdsa::ecdsa (const elliptic_curve* _curve) : 
	digit_signature_t (signature::ECDSA, "ECDSA-" + _curve->get_name (), 
			_curve->get_field ().sizeinbase (2)), curve (_curve) { }

ecdsa::public_key_t::public_key_t (const elliptic_curve* _curve, const point& _Q)
	: curve (_curve), Q (_Q) {
}

ecdsa::public_key_t::~public_key_t () { }

bool ecdsa::public_key_t::check_sign (const long_number_t& m, const digit_signature_t::sign_t& s) const {
	if (!s.verify (signature::ECDSA)) {
		return false;
	}
	const sign_t& _s = (const sign_t&)s;
	const long_number_t R = _s.get_r (), S = _s.get_s ();
	const long_number_t n = curve->get_n ();
	if (!(long_number_t{1} <= R && R < n && long_number_t{1} <= S && S < n)) {
		return false;
	}
	const long_number_t w = modular_invert (S, n);
	const long_number_t u1 = (m * w) % n, u2 = (R * w) % n;
	const point x = curve->sum (curve->mul_generator (u1), curve->mul (Q, u2));
	if (x.inf ()) {
		return false;
	}
	const long_number_t v = x.get_x () % n;
	return v == R;
}

ecdsa::private_key_t::private_key_t (const elliptic_curve* _curve, const long_number_t& _d)
	: curve (_curve), d (_d) {
}

ecdsa::private_key_t::~private_key_t () { }
wrapper<digit_signature_t::sign_t> ecdsa::private_key_t::sign (const long_number_t& m) const {
	const long_number_t n = curve->get_n ();
	const _size_t key_size = (n.sizeinbase (2) + 7) / 8 + 1;
	long_number_t k, r, s;
	point p{true};
	while (true) {
		k = gen_randint<true> (key_size) % (n - 2) + 2;
		p = curve->mul_generator (k);
		r = p.get_x () % n;
		if (r == 0) {
			continue;
		}
		s = (modular_invert (k, n) * (m + d * r)) % n;
		if (s == 0) {
			continue;
		}
		return wrapper<digit_signature_t::sign_t> (new sign_t (r, s));
	}
}

ecdsa::~ecdsa () { }
std::pair<wrapper<digit_signature_t::private_key_t>, wrapper<digit_signature_t::public_key_t>> ecdsa::generate_keys () const {
	const long_number_t n = curve->get_n ();
	const long_number_t d = gen_randint<true> ((n.sizeinbase (2) + 7) / 8 + 1) % (n - 2) + 2;
	const point Q = curve->mul_generator (d);
	return {wrapper<digit_signature_t::private_key_t>(new private_key_t (curve, d)),
		wrapper<digit_signature_t::public_key_t> (new public_key_t (curve, Q))};
}
static std::vector<wrapper<ecdsa>> instances {};
void ecdsa::register_all () {
	if (instances.size () == 0) {
		for (auto it : elliptic_curve::get_possibles ()) {
			instances.push_back (wrapper<ecdsa> (new ecdsa (it.value ().get ())));
		}
	}
}
vector_view<wrapper<ecdsa>> ecdsa::available () {
	static vector_view<wrapper<ecdsa>> _avail {instances};
	return _avail;
}

_ADD_LINK (ecdsa, register_all, ecdsa, owner<elliptic_curve>().id);
_ADD_2(ecdsa, ecdsa);

#endif 
