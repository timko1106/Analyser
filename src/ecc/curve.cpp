#ifndef ECC_CURVE_CPP_
#define ECC_CURVE_CPP_

#include "../../include/analyser/ecc/curve.hpp"

curve::curve (const long_number_t& _p, const long_number_t& _a, const long_number_t& _b) : p (_p), a (_a), b (_b) { }
curve::~curve () { }
long_number_t& curve::mod (long_number_t& val) const {
	return val %= p;
}
bool curve::check_curve () const {
	return (long_number_t(4) * a * a * a + long_number_t(27) * b * b) != long_number_t(0);
}
point curve::sum (const point& P, const point& Q) const {
	if (P.inf () && Q.inf ()) {
		return point (true);
	}
	point p_inv = invert (P), q_inv = invert (Q);
	if (Q == p_inv || P == q_inv) {
		return point (true);
	}
	if (P.inf ()) {
		return Q;
	}
	if (Q.inf ()) {
		return P;
	}
	if (P == Q) {
		return mul_2 (P);
	}
	long_number_t dy = Q.get_y () - P.get_y (), dx = Q.get_x () - P.get_x ();
	mod (dy);
	mod (dx);
	long_number_t k = dy * modular_invert (dx, this->p);
	mod (k);
	long_number_t k_2 = k * k;
	mod (k_2);
	long_number_t x_r = k_2 - P.get_x () - Q.get_x ();
	long_number_t y_r = -(Q.get_y () + k * (x_r - Q.get_x ()));
	mod (x_r);
	mod (y_r);
	return point (x_r, y_r); 
}
point curve::mul_2 (const point& P) const {
	point inv = invert (P);
	if(P.inf () || (inv == P)) {
		return point (true);
	}
	long_number_t k = (long_number_t (3) * P.get_x () * P.get_x () + a) * modular_invert (long_number_t(2) * P.get_y (), p);
	mod (k);
	long_number_t x_r = k * k - long_number_t (2) * P.get_x ();
	mod (x_r);
	long_number_t y_r = -(P.get_y () + k * (x_r - P.get_x ()));
	mod (y_r);
	return point (x_r, y_r);
}
point curve::mul (const point& P, const long_number_t& cnt) const {
	if (P.inf ()) {
		return point (true);
	}
	point _p = P, R = point (true);
	long_number_t k = cnt;
	while (k != 0) {
		if (k % 2 == 1) {
			R = this->sum (R, _p);
		}
		_p = mul_2 (_p);
		k >>= 1;
	}
	return R;
}
point curve::invert (const point& P) const {
	if (P.inf ()) {
		return point (true);
	}
	return point (P.get_x (), -P.get_y ());
}
bool curve::on_curve (const point& P) const {
	if (P.inf ()) {
		return true;
	}
	auto left = P.get_y () * P.get_y (), right = P.get_x () * P.get_x () * P.get_x () + this->a * P.get_x () + this->b;
	return mod (left) == mod (right);
}

long_number_t curve::get_field () const {
	return p;
}

#endif
