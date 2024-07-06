#ifndef ASYMMETRIC_DSA_CPP_
#define ASYMMETRIC_DSA_CPP_

#include "../../include/analyser/asymmetric/dsa.hpp"

static std::vector<wrapper<dsa>> instances{};

dsa::dsa (_size_t _N, _size_t _L) : 
	digit_signature_t (signature::DSA, 
			"DSA-" + std::to_string (_N) + "," + std::to_string (_L),
			_N), N (_N), L (_L) { }
dsa::public_key_t::public_key_t (const long_number_t& _p, const long_number_t& _q
		, const long_number_t& _g, const long_number_t& _y) : p (_p), q (_q), g (_g), y (_y) { }
dsa::public_key_t::~public_key_t () { }
bool dsa::public_key_t::check_sign (const long_number_t& m, const digit_signature_t::sign_t& s) const {
	if (!s.verify (signature::DSA)) {
		return false;
	}
	const sign_t& _s = (const sign_t&)s;
	const long_number_t r = _s.get_r (), S = _s.get_s ();
	if (!(long_number_t (0) < r && r < q && long_number_t (0) < S && S < q)) {
		return false;
	}
	const long_number_t w = modular_invert (S, q);
	const long_number_t u1 = (m * w) % q;
	const long_number_t u2 = (r * w) % q;
	const long_number_t v = ((pow_m (g, u1, p) * pow_m (y, u2, p)) % p) % q;
	return v == r;
}

dsa::private_key_t::private_key_t (const long_number_t& _p, const long_number_t& _g, 
		const long_number_t& _q, const long_number_t& _x) : p (_p), g (_g)
								    , q (_q), x (_x) {}
dsa::private_key_t::~private_key_t () { }
wrapper<digit_signature_t::sign_t> dsa::private_key_t::sign (const long_number_t& m) const {
	long_number_t k, r, s;
	const _size_t len = (q.sizeinbase (2) + 7) / 8 + 1;
	while (true) {
		k = gen_randint<true>(len) % (q - 2) + 2;
		//k_max = q - 3 + 2 = q - 1
		//k_min = 0 + 2 = 2
		r = pow_m (g, k, p) % q;
		if (r == 0) {
			continue;
		}
		s = (modular_invert (k, q) * (m + x * r)) % q;
		if (s == 0) {
			continue;
		}
		return wrapper<digit_signature_t::sign_t> (new sign_t (r, s));
	}
}

dsa::~dsa () { }
std::pair<wrapper<digit_signature_t::private_key_t>, wrapper<digit_signature_t::public_key_t>> dsa::generate_keys () const {
	const dsa_params p = generation (L, N);
	const long_number_t x = gen_randint<true> ((p.q.sizeinbase (2) + 7) / 8) % (p.q - 3) + 2;
	const long_number_t y = pow_m (p.g, x, p.p);
	return {wrapper<digit_signature_t::private_key_t>(new private_key_t (p.p, p.g, p.q, x)),
		wrapper<digit_signature_t::public_key_t>(new public_key_t (p.p, p.q, p.g, y))};
}
void dsa::register_all () {
	if (instances.size () == 0) {
		static _size_t recommended[][2] = {
			{1024, 160},
			{2048, 224},
			{2048, 256},
			{3072, 256}};
		const _size_t COUNT = sizeof (recommended) / sizeof (*recommended);
		for (_size_t i = 0; i < COUNT; ++i) {
			instances.push_back (wrapper<dsa>(new dsa (recommended[i][1], recommended[i][0])));
		}
	}
}
vector_view<wrapper<dsa>> dsa::available () {
	static vector_view<wrapper<dsa>> view {instances};
	return view;
}
_ADD_LINK_ (dsa, register_all, dsa);
_ADD_2(dsa, dsa);

#endif
