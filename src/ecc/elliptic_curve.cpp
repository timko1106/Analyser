#ifndef ECC_ELLIPTIC_CURVE_CPP_
#define ECC_ELLIPTIC_CURVE_CPP_

#include "../../include/analyser/ecc/elliptic_curve.hpp"

std::map<std::string, wrapper<elliptic_curve>>& elliptic_curve::get_values () {
	static std::map<std::string, wrapper<elliptic_curve>> registered{};
	return registered;
}

elliptic_curve::elliptic_curve (const std::string& _name, const point& _g, const long_number_t& _p, const long_number_t& _a, const long_number_t& _b) : curve (_p, _a, _b), name (_name), G (_g) { }

elliptic_curve::~elliptic_curve () {
	//get_values ().erase (name);
}
void elliptic_curve::deregister () const {
	get_values ().erase (name);
}
const elliptic_curve* elliptic_curve::get (const std::string& name) {
	auto it = get_values ().find (name);
	if (it == get_values ().end ()) {
		return nullptr;
	}
	return it->second.get ();
}
const elliptic_curve* elliptic_curve::register_curve (const std::string& _name, const point& g, const long_number_t& p, const long_number_t& a, const long_number_t& b) {
	if (get_values ().find (_name) != get_values ().end ()) {
		return nullptr;//registered 
	}
	elliptic_curve* ptr = new elliptic_curve (_name, g, p, a, b);
	if (!ptr->check_curve () || !ptr->on_curve (g)) {
		delete ptr;
		return nullptr;
	}
	get_values ()[_name] = wrapper<elliptic_curve> (ptr);
	return ptr;
}
point elliptic_curve::mul_generator (const long_number_t& k) const {
	return this->mul (G, k);
}
std::string elliptic_curve::get_name () const {
	return name;
}
const map_view<std::map<std::string, wrapper<elliptic_curve>>>& elliptic_curve::get_possibles () {
	static map_view<std::map<std::string, wrapper<elliptic_curve>>> vals{get_values()};
	return vals;
}
void elliptic_curve::register_all () {
	UNUSED static elliptic_curve::ec_ptr curves[] = {secp256k1, p_192, p_224, p_256, p_384, p_521};
}
const elliptic_curve *elliptic_curve::secp256k1 = elliptic_curve::register_curve ("Secp256k1", 
	point (long_number_t ("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798"), 
		long_number_t ("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8")), 
	long_number_t ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F"), 
	0, 7);
const elliptic_curve *elliptic_curve::p_192 = elliptic_curve::register_curve ("P192",
		point (long_number_t ("188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012"),
			long_number_t ("07192b95ffc8da78631011ed6b24cdd573f977a11e794811")),
		long_number_t ("fffffffffffffffffffffffffffffffeffffffffffffffff"),
		-3, long_number_t ("64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1"));
const elliptic_curve *elliptic_curve::p_224 = elliptic_curve::register_curve ("P224",
		point (long_number_t ("b70e0cbd6bb4bf7f321390b94a03c1d356c21122343280d6115c1d21"),
			long_number_t ("bd376388b5f723fb4c22dfe6cd4375a05a07476444d5819985007e34")),
		long_number_t ("ffffffffffffffffffffffffffffffff000000000000000000000001"),
		-3, long_number_t ("b4050a850c04b3abf54132565044b0b7d7bfd8ba270b39432355ffb4"));
const elliptic_curve *elliptic_curve::p_256 = elliptic_curve::register_curve ("P256",
		point (long_number_t ("6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296"),
			long_number_t ("4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5")),
		long_number_t ("ffffffff00000001000000000000000000000000ffffffffffffffffffffffff"),
		-3, long_number_t ("5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b"));
const elliptic_curve* elliptic_curve::p_384 = elliptic_curve::register_curve ("P384",
		point (long_number_t ("aa87ca22be8b05378eb1c71ef320ad746e1d3b628ba79b9859f741e082542a385502f25dbf55296c3a545e3872760ab7"),
			long_number_t ("3617de4a96262c6f5d9e98bf9292dc29f8f41dbd289a147ce9da3113b5f0b8c00a60b1ce1d7e819d7a431d7c90ea0e5f")),
		long_number_t ("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff0000000000000000ffffffff"),
		-3, long_number_t ("b3312fa7e23ee7e4988e056be3f82d19181d9c6efe8141120314088f5013875ac656398d8a2ed19d2a85c8edd3ec2aef"
			));
const elliptic_curve* elliptic_curve::p_521 = elliptic_curve::register_curve ("P521",
		point (long_number_t ("00c6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe75928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66"),
			long_number_t ("011839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef42640c550b9013fad0761353c7086a272c24088be94769fd16650")),
		long_number_t ("01ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"),
		-3, long_number_t ("0051953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00"));

_ADD_LINK_ (ec, register_all, elliptic_curve);
_ADD_2(ec, elliptic_curve);


#endif
