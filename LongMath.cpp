#ifndef LONG_MATH_CPP
#define LONG_MATH_CPP

#include "LongMath.hpp"

long_number_t::long_number_t (const char* hex_view, int base) : num (hex_view, base) { }

long_number_t::long_number_t (const std::string& s, int base) : num (s, base) { }

long_number_t::long_number_t (const long_number_t& other) : num (other.num) { }

long_number_t::long_number_t (long_number_t&& other) : num (std::forward<mpz_class>(other.num)) { }

long_number_t& long_number_t::operator+= (const long_number_t& other) {
	num += other.num;
	return *this;
}
long_number_t long_number_t::operator+ (const long_number_t& other) const {
	long_number_t res = *this;
	res += other;
	return res;
}

long_number_t& long_number_t::operator%= (const long_number_t& other) {
	num %= other.num;
	return *this;
}
long_number_t long_number_t::operator% (const long_number_t& other) const {
	long_number_t res = *this;
	res %= other;
	return res;
}

long_number_t& long_number_t::operator-= (const long_number_t& other) {
	num -= other.num;
	return *this;
}
long_number_t long_number_t::operator- (const long_number_t& other) const {
	long_number_t res = *this;
	res -= other;
	return res;
}

long_number_t& long_number_t::operator*= (const long_number_t& other) {
	num *= other.num;
	return *this;
}

long_number_t long_number_t::operator* (const long_number_t& other) const {
	long_number_t res = *this;
	res *= other;
	return res;
}


long_number_t& long_number_t::operator<<= (shift_t shift) {
	num <<= shift;
	return *this;
}

long_number_t long_number_t::operator<< (shift_t shift) const {
	long_number_t res = *this;
	res <<= shift;
	return res;
}

long_number_t& long_number_t::operator>>= (shift_t shift) {
	num >>= shift;
	return *this;
}

long_number_t long_number_t::operator>> (shift_t shift) const {
	long_number_t res = *this;
	res >>= shift;
	return res;
}

long_number_t& long_number_t::operator|= (const long_number_t& other) {
	num |= other.num;
	return *this;
}

long_number_t long_number_t::operator| (const long_number_t& other) const {
	long_number_t res = *this;
	res |= other;
	return res;
}

long_number_t& long_number_t::operator&= (const long_number_t& other) {
	num &= other.num;
	return *this;
}

long_number_t long_number_t::operator& (const long_number_t& other) const {
	long_number_t res = *this;
	res &= other;
	return res;
}

long_number_t& long_number_t::operator^= (const long_number_t& other) {
	num |= other.num;
	return *this;
}

long_number_t long_number_t::operator^ (const long_number_t& other) const {
	long_number_t res = *this;
	res |= other;
	return res;
}



long_number_t& long_number_t::operator= (const long_number_t& other) {
	num = other.num;
	return *this;
}

long_number_t& long_number_t::operator= (long_number_t&& other) {
	num = std::forward<mpz_class>(other.num);
	return *this;
}

long_number_t::operator std::string() const {
	return num.get_str (BASE);
}
std::string long_number_t::get (int base) const {
	return num.get_str (base);
}

long_number_t gen_randint (_size_t bytes) {
	long_number_t gen{};
	std::random_device seed_gen{};
	gen = 0;
	const _size_t RAND_SIZE = sizeof (std::random_device::result_type);
	long_number_t tmp = 1;
	tmp <<= (8 * RAND_SIZE);
	for (_size_t i = 0; i < bytes / RAND_SIZE; ++i) {
		gen *= tmp;
		gen += seed_gen ();
	}
	return gen;
}

long_number_t gen_randprime (_size_t bytes, bool round) {
	long_number_t gen = gen_randint (bytes), get{};
	if (round) {
		long_number_t min = long_number_t (1) << (8 * bytes - 1);
		while (gen < min) {
			gen <<= 1;
		}
	}
	mpz_nextprime (get.num.get_mpz_t (), gen.num.get_mpz_t ());
	return get;
}

long_number_t modular_invert (const long_number_t& what, const long_number_t& mod) {
	long_number_t result{};
	//Расширенный алгоритм Евклида.
	//https://github.com/bennoleslie/gmp/blob/master/mpz/invert.c#L48
	//https://github.com/bennoleslie/gmp/blob/master/mpz/gcdext.c 
	mpz_invert (result.num.get_mpz_t (), what.num.get_mpz_t (), mod.num.get_mpz_t ());
	return result;
}

long_number_t pow_prime_m (const long_number_t& base, const long_number_t& exp, const long_number_t& mod) {
	long_number_t result{}, exp_optimized = exp % (mod - 1);
	mpz_powm (result.num.get_mpz_t (), base.num.get_mpz_t (), exp_optimized.num.get_mpz_t (), mod.num.get_mpz_t ());
	return result;
}

long_number_t gcd (const long_number_t& a, const long_number_t& b) {
	//Чисто алгоритм Евклида.
	long_number_t result {};
	mpz_gcd (result.num.get_mpz_t (), a.num.get_mpz_t (), b.num.get_mpz_t ());
	return result;
}

long_number_t lcm (const long_number_t& a, const long_number_t& b) {
	//Фактически a/gcd*b
	long_number_t result {};
	mpz_lcm (result.num.get_mpz_t (), a.num.get_mpz_t (), b.num.get_mpz_t ());
	return result;
}


#endif
