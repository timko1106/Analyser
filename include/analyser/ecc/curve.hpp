#ifndef ECC_CURVE_H_
#define ECC_CURVE_H_

#include "point.hpp"
#include "../base/long_math.hpp"
#include "../base/vars.hpp"

class curve {
	const long_number_t p, a, b;
	//y^2=x^3+ax+b.
	long_number_t& mod (long_number_t&) const;
public:
	curve (const long_number_t& _p, const long_number_t& _a, const long_number_t& _b);
	BLOCK_COPYING (curve);
	virtual ~curve ();
	bool check_curve () const;
	point sum (const point& P, const point& Q) const;
	point mul_2 (const point& P) const;
	point mul (const point& P, const long_number_t& cnt) const;
	point invert (const point& P) const;
	bool on_curve (const point& P) const;
	long_number_t get_field () const;
};

#endif
