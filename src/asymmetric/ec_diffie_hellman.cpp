#ifndef ASYMMETRIC_EC_DIFFIE_HELLMAN_CPP_
#define ASYMMETRIC_EC_DIFFIE_HELLMAN_CPP_

#include "../../include/analyser/asymmetric/ec_diffie_hellman.hpp"

ec_diffie_hellman::ec_diffie_hellman (const elliptic_curve* _curve) : asymmetric_exchange_t (asymmetric_exchange::EC_DIFFIE_HELLMAN, "ECDiffieHellman-" + _curve->get_name (), _curve->get_field ().sizeinbase (2)), curve (_curve) { }
ec_diffie_hellman::~ec_diffie_hellman () { }
ec_diffie_hellman::key_t::key_t (const public_key_t& _val) : asymmetric_exchange_t::key_t (_val.get_curve ()->get_field ().sizeinbase (2)), val (_val), A (true) {
	while (A.inf ()) {
		a = gen_randint<true> ((this->size () + 7) / 8);
		A = val.get_curve ()->mul (val.get_b (), a);
	}
}
ec_diffie_hellman::key_t::key_t (const public_key_t& _val, const message_t& _m) : asymmetric_exchange_t::key_t (_val.get_curve ()->get_field ().sizeinbase (2)), val (_val), a (0), A (true) {
	point _A = _m.get_val (), B = _val.get_b ();
	while (A.inf () || _val.get_curve ()->mul (_A, a).inf ()) {
		a = gen_randint<true> ((this->size () + 7) / 8);
		A = val.get_curve ()->mul (B, a);
	}
}
ec_diffie_hellman::key_t::~key_t () { }
wrapper<asymmetric_exchange_t::message_t> ec_diffie_hellman::key_t::gen_A () const {
	return wrapper<asymmetric_exchange_t::message_t>(new message_t (A));
}
wrapper<asymmetric_exchange_t::message_t> ec_diffie_hellman::key_t::gen_K (const asymmetric_exchange_t::message_t& other) const {
	if (!other.verify (asymmetric_exchange::EC_DIFFIE_HELLMAN)) {
		return nullptr;
	}
	point _A = ((const message_t&)other).get_val ();
	return wrapper<asymmetric_exchange_t::message_t>(new message_t (this->val.get_curve ()->mul (_A, a)));
}
wrapper<asymmetric_exchange_t::public_key_t> ec_diffie_hellman::key_t::get_public () const {
	return wrapper<asymmetric_exchange_t::public_key_t>(new public_key_t (val));
}

wrapper<asymmetric_exchange_t::key_t> ec_diffie_hellman::gen_key (UNUSED _size_t keysize) const {
	return gen_default_key ();
}
wrapper<asymmetric_exchange_t::key_t> ec_diffie_hellman::gen_default_key () const {
	long_number_t c = gen_randint<true> ((default_key_size () + 7) / 8);
	point B = this->curve->mul_generator (c);
	return wrapper<asymmetric_exchange_t::key_t>(new key_t (ec_diffie_hellman::public_key_t (B, curve)));
}
wrapper<asymmetric_exchange_t::key_t> ec_diffie_hellman::gen_other (const asymmetric_exchange_t::message_t& A, const asymmetric_exchange_t::public_key_t& pub) const {
	if (!pub.verify (asymmetric_exchange::EC_DIFFIE_HELLMAN) || !A.verify (asymmetric_exchange::EC_DIFFIE_HELLMAN)) {
		return nullptr; 
	}
	return wrapper<asymmetric_exchange_t::key_t>(new key_t ((const public_key_t&)pub, (const message_t&)A));
}
static std::vector<wrapper<ec_diffie_hellman>> data{};

const vector_view<wrapper<ec_diffie_hellman>> ec_diffie_hellman::get_instances () {
	static vector_view<wrapper<ec_diffie_hellman>> view{data};
	return view;
}

void ec_diffie_hellman::register_all () {
	if (data.size () == 0) {
		for (auto it : elliptic_curve::get_possibles ()) {
			data.push_back (wrapper<ec_diffie_hellman>(new ec_diffie_hellman (it.value ().get ())));
		}
	}
}
_ADD_LINK (ec_dh, register_all, ec_diffie_hellman, owner<elliptic_curve>().id);
_ADD_2(ec_dh, ec_diffie_hellman);


#endif
