#ifndef BASE_LONG_MATH_H_
#define BASE_LONG_MATH_H_

#include "vars.hpp"
#include "random_source.hpp"
#include <gmpxx.h>
#include <type_traits>
#include <limits>
#include <vector>

constexpr int BASE = 16;

using shift_t = unsigned;
struct dsa_params;

class long_number_t {
	mpz_class num;
public:
	long_number_t (const char* hex_view, int base = BASE);
	long_number_t (const std::string& hex_view, int base = BASE);
	long_number_t (const long_number_t& other);
	long_number_t (long_number_t&& other);
	long_number_t (const mpz_class& _num);
	long_number_t () { }
	const mpz_class& raw () const;

	~long_number_t () { }

	template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
	long_number_t (const T& val = T()) : num (val) { }

	long_number_t operator- () const;

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

	bool operator< (const long_number_t& other) const;
	bool operator<= (const long_number_t& other) const;
	bool operator> (const long_number_t& other) const;
	bool operator>= (const long_number_t& other) const;
	bool operator== (const long_number_t& other) const;
	bool operator!= (const long_number_t& other) const;

	explicit operator std::string() const;
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
	_size_t sizeinbase (int base) const;

	template<bool secure>
	friend long_number_t gen_randint (_size_t bytes);

	friend long_number_t gen_next_prime (const long_number_t& n);
	friend long_number_t gen_randprime (_size_t bytes, bool round);
	friend long_number_t modular_invert (const long_number_t& what, const long_number_t& mod);
	friend long_number_t pow_prime_m (const long_number_t& base, const long_number_t& exp, const long_number_t& mod);
	friend long_number_t pow_m (const long_number_t& base, const long_number_t& exp, const long_number_t& mod);
	friend long_number_t gcd (const long_number_t& a, const long_number_t& b);
	friend long_number_t lcm (const long_number_t& a, const long_number_t& b);
	friend int probably_prime (const long_number_t& n);
	friend std::pair<long_number_t, long_number_t> gen_prime_and_root (_size_t bytes);
	friend dsa_params generation (_size_t L, _size_t N);
};
template<bool secure>
long_number_t gen_randint (_size_t bytes);
long_number_t gen_next_prime (const long_number_t& n);
//round - добить так, чтобы произведение двух rand_prime имело >b1+b2 байт.
long_number_t gen_randprime (_size_t bytes, bool round = false);
long_number_t modular_invert (const long_number_t& what, const long_number_t& mod);
long_number_t pow_prime_m (const long_number_t& base, const long_number_t& exp, const long_number_t& mod);
long_number_t pow_m (const long_number_t& base, const long_number_t& exp, const long_number_t& mod);
long_number_t gcd (const long_number_t& a, const long_number_t& b);
long_number_t lcm (const long_number_t& a, const long_number_t& b);
int probably_prime (const long_number_t& n);
struct dsa_params {
	long_number_t p, q, g;
};
dsa_params generation (_size_t L, _size_t N);
std::pair<long_number_t, long_number_t> gen_prime_and_root (_size_t bits);

#endif
