#ifndef LONG_MATH_H
#define LONG_MATH_H

#include "Vars.hpp"
#include <gmpxx.h>
#include <random>
#include <type_traits>
#include <limits>

constexpr int BASE = 16;

using shift_t = unsigned;

class long_number_t {
	mpz_class num;
public:
	long_number_t (const char* hex_view, int base = BASE);
	long_number_t (const std::string& hex_view, int base = BASE);
	long_number_t (const long_number_t& other);
	long_number_t (long_number_t&& other);
	long_number_t () { }

	~long_number_t () { }

	template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
	long_number_t (const T& val = T()) : num (val) { }

	long_number_t& operator+= (const long_number_t& other);
	long_number_t operator+ (const long_number_t& other) const;

	long_number_t& operator*= (const long_number_t& other);
	long_number_t operator* (const long_number_t& other) const;

	long_number_t& operator%= (const long_number_t& other);
	long_number_t operator% (const long_number_t& other) const;

	long_number_t& operator-= (const long_number_t& other);
	long_number_t operator- (const long_number_t& other) const;

	long_number_t& operator<<= (shift_t shift);
	long_number_t operator<< (shift_t shift) const;

	long_number_t& operator>>= (shift_t shift);
	long_number_t operator>> (shift_t shift) const;

	long_number_t& operator|= (const long_number_t& other);
	long_number_t operator| (const long_number_t& other) const;

	long_number_t& operator^= (const long_number_t& other);
	long_number_t operator^ (const long_number_t& other) const;

	long_number_t& operator&= (const long_number_t& other);
	long_number_t operator& (const long_number_t& other) const;

	long_number_t& operator= (const long_number_t& other);
	long_number_t& operator= (long_number_t&& other);

#define COMPARE_OP(op_name, check) \
	bool op_name (const long_number_t& other) const { \
		return num check other.num; \
	}
	COMPARE_OP (operator<, <);
	COMPARE_OP (operator<=, <=);
	COMPARE_OP (operator>, >);
	COMPARE_OP (operator>=, >=);
	COMPARE_OP (operator==, ==);
	COMPARE_OP (operator!=, !=);
#undef COMPARE_OP

	operator std::string() const;
	std::string get (int base = BASE) const;
	template<typename T>
	T get () const;
	template<long>
	long get () const {
		return num.get_si ();
	}
	template<unsigned long>
	unsigned long get () const {
		return num.get_ui ();
	}
	friend long_number_t gen_randint (_size_t bytes);
	friend long_number_t gen_randprime (_size_t bytes, bool round);
	friend long_number_t modular_invert (const long_number_t& what, const long_number_t& mod);
	friend long_number_t pow_prime_m (const long_number_t& base, const long_number_t& exp, const long_number_t& mod);
	friend long_number_t gcd (const long_number_t& a, const long_number_t& b);
	friend long_number_t lcm (const long_number_t& a, const long_number_t& b);
};

long_number_t gen_randint (_size_t bytes);
//round - добить так, чтобы произведение двух rand_prime имело >b1+b2 байт.
long_number_t gen_randprime (_size_t bytes, bool round = false);
long_number_t modular_invert (const long_number_t& what, const long_number_t& mod);
long_number_t pow_prime_m (const long_number_t& base, const long_number_t& exp, const long_number_t& mod);
long_number_t gcd (const long_number_t& a, const long_number_t& b);
long_number_t lcm (const long_number_t& a, const long_number_t& b);

#endif
