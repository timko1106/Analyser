#ifndef ECC_POINT_H_
#define ECC_POINT_H_

#include "../base/long_math.hpp"

class point {
	long_number_t x, y;
	bool infinity;
public:
	point (bool _inf);
	point (const long_number_t& _x, const long_number_t& _y);
	point (const point& p);
	const long_number_t& get_x () const;
	long_number_t& get_x ();
	const long_number_t& get_y () const;
	long_number_t& get_y ();
	void set_inf (bool val);
	bool inf () const;
	point& operator= (const point& p);
	bool operator== (const point& p) const;
};

#endif
