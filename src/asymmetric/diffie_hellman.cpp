#ifndef ASYMMETRIC_DIFFIE_HELLMAN_CPP_
#define ASYMMETRIC_DIFFIE_HELLMAN_CPP_

#include "../../include/analyser/asymmetric/diffie_hellman.hpp"

diffie_hellman::diffie_hellman () : asymmetric_exchange_t (asymmetric_exchange::DIFFIE_HELLMAN, "DiffieHellman", 2048) { }

diffie_hellman::key_t::key_t (const public_key_t& _val) : asymmetric_exchange_t::key_t(_val.get_p ().sizeinbase (2)), val (_val) {
	long_number_t p = val.get_p (), g = val.get_g ();
	_size_t bytes = (p.sizeinbase (2) + 7) / 8;
	a = gen_randint<true>(bytes + 1) % (p - 2) + 2;
	A = pow_prime_m (g, a, p);
	while (A == 1) {
		a = gen_randint<true>(bytes + 1) % (p - 2) + 2;
		A = pow_prime_m (g, a, p); 
	}
}

diffie_hellman::key_t::key_t (const public_key_t& _val, const message_t& other) : asymmetric_exchange_t::key_t (_val.get_p ().sizeinbase (2)), val (_val) {
	long_number_t p = val.get_p (), g = val.get_g (), _A = other.get_val ();
	_size_t bytes = (p.sizeinbase (2) + 7) / 8;
	a = gen_randint<true>(bytes + 1) % (p - 2) + 2;
	A = pow_prime_m (g, a, p);
	long_number_t _K = pow_prime_m (_A, a, p);
	while (A == 1 || _K == 1) {
		a = gen_randint<true>(bytes + 1) % (p - 2) + 2;
		A = pow_prime_m (g, a, p);
		_K = pow_prime_m (_A, a, p);
	}
}

diffie_hellman::key_t::~key_t () { }
wrapper<asymmetric_exchange_t::message_t> diffie_hellman::key_t::gen_A () const {
	return wrapper<asymmetric_exchange_t::message_t>(new message_t (A));
}
wrapper<asymmetric_exchange_t::message_t> diffie_hellman::key_t::gen_K (const asymmetric_exchange_t::message_t& other) const {
	if (!other.verify (asymmetric_exchange::DIFFIE_HELLMAN)) {
		return nullptr;
	}
	long_number_t _A = ((const message_t&)other).get_val ();
	long_number_t K = pow_prime_m (_A, a, val.get_p ());
	return wrapper<asymmetric_exchange_t::message_t>{(asymmetric_exchange_t::message_t*)new message_t (K)};
}
wrapper<asymmetric_exchange_t::public_key_t> diffie_hellman::key_t::get_public () const {
	return wrapper<asymmetric_exchange_t::public_key_t>(new public_key_t (val));
}
diffie_hellman::~diffie_hellman () { }
wrapper<asymmetric_exchange_t::key_t> diffie_hellman::gen_key (_size_t bits) const {
	auto pair = gen_prime_and_root (bits);
	long_number_t p = pair.first;
	long_number_t g = pair.second;
	return wrapper<asymmetric_exchange_t::key_t> (new key_t (public_key_t (p, g)));
}
wrapper<asymmetric_exchange_t::key_t> diffie_hellman::gen_default_key () const {
	return gen_key (this->default_key_size ());
}
wrapper<asymmetric_exchange_t::key_t> diffie_hellman::gen_other (const asymmetric_exchange_t::message_t& A, const asymmetric_exchange_t::public_key_t& pub) const {
	if (!A.verify (asymmetric_exchange::DIFFIE_HELLMAN) || !pub.verify (asymmetric_exchange::DIFFIE_HELLMAN)) {
		return nullptr;
	}
	const message_t& m = (const message_t&) A;
	const public_key_t& p = (const public_key_t&) pub;
	return wrapper<asymmetric_exchange_t::key_t>(new key_t (p, m));
}
const diffie_hellman diffie_hellman::instance {};

#endif
