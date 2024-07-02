#ifndef ECC_POINT_CPP_
#define ECC_POINT_CPP_

#include "../../include/analyser/ecc/point.hpp"

point::point (bool inf) : x (0), y (0), infinity (inf) { }
point::point (const long_number_t& _x, const long_number_t& _y) : x (_x), y (_y), infinity (false) { }
point::point (const point& p) : x (p.x), y (p.y), infinity (p.infinity) { }
const long_number_t& point::get_x () const {
	return x;
}
long_number_t& point::get_x () {
	return x;
}
const long_number_t& point::get_y () const {
	return y;
}
long_number_t& point::get_y () {
	return y;
}
void point::set_inf (bool val) {
	infinity = val;
}
bool point::inf () const {
	return infinity;
}
point& point::operator= (const point& p) {
	infinity = p.infinity;
	x = p.x;
	y = p.y;
	return *this;
}
bool point::operator== (const point& p) const {
	return (infinity ? p.infinity : x == p.x && y == p.y);
}

#endif
