#ifndef LONG_MATH_CPP
#define LONG_MATH_CPP

#include "long_math.hpp"

long_number_t::long_number_t (const char* hex_view, int base) : num (hex_view, base) { }

long_number_t::long_number_t (const std::string& s, int base) : num (s, base) { }

long_number_t::long_number_t (const long_number_t& other) : num (other.num) { }

long_number_t::long_number_t (long_number_t&& other) : num (std::forward<mpz_class>(other.num)) { }

#define ASSIGN_OPERATOR(name, op) \
	long_number_t& long_number_t::name (const long_number_t& other) { \
		num op other.num; \
		return *this; \
	}
#define BIN_OPERATOR(name, op) \
	long_number_t long_number_t::name (const long_number_t& other) const { \
		long_number_t res = *this; \
		res op other; \
		return res; \
	}

long_number_t long_number_t::operator- () const {
	long_number_t res = *this;
	res.num = -res.num;
	return res;
}
ASSIGN_OPERATOR (operator+=, +=);
BIN_OPERATOR (operator+, +=);

long_number_t& long_number_t::operator%= (const long_number_t& other) {
	mpz_class _abs = abs (other.num);
	((num %= _abs) += _abs) %= _abs;
	return *this;
}
BIN_OPERATOR (operator%, %=);

ASSIGN_OPERATOR(operator-=, -=);
BIN_OPERATOR(operator-, -=);

ASSIGN_OPERATOR(operator*=, *=);
BIN_OPERATOR(operator*, *=);

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

ASSIGN_OPERATOR(operator|=, |=);
BIN_OPERATOR(operator|, |=);

ASSIGN_OPERATOR(operator&=, &=);
BIN_OPERATOR(operator&, &=);

ASSIGN_OPERATOR(operator^=, ^=);
BIN_OPERATOR(operator^, ^=);
#undef ASSIGN_OPERATOR
#undef BIN_OPERATOR

long_number_t& long_number_t::operator= (const long_number_t& other) {
	num = other.num;
	return *this;
}

long_number_t& long_number_t::operator= (long_number_t&& other) {
	num = std::forward<mpz_class>(other.num);
	return *this;
}

#define COMP_OPERATOR(name, op) \
	bool long_number_t::name (const long_number_t& other) const { \
		return num op other.num; \
	}
COMP_OPERATOR (operator<, <);
COMP_OPERATOR (operator<=, <=);
COMP_OPERATOR (operator>, >);
COMP_OPERATOR (operator>=, >=);
COMP_OPERATOR (operator==, ==);
COMP_OPERATOR (operator!=, !=);
#undef COMP_OPERATOR


long_number_t::operator std::string() const {
	return num.get_str (BASE);
}
std::string long_number_t::get (int base) const {
	return num.get_str (base);
}

_size_t long_number_t::sizeinbase (int base) const {
	return mpz_sizeinbase (num.get_mpz_t (), base);
}

template<>
long_number_t gen_randint<true> (_size_t bytes) {
	long_number_t gen{};
	gen = 0;
	const _size_t RAND_SIZE = random_generator::TYPE_SIZE;
	long_number_t tmp = 1;
	tmp <<= (8 * RAND_SIZE);
	for (_size_t i = 0; i < (bytes + RAND_SIZE - 1) / RAND_SIZE; ++i) {
		gen *= tmp;
		gen += random_generator::get_secure ();
	}
	for (_size_t i = 0; i < (RAND_SIZE - bytes % RAND_SIZE) % RAND_SIZE; ++i) {
		gen >>= 8;
	}
	return gen;
}
template<>
long_number_t gen_randint<false> (_size_t bytes) {
	long_number_t gen{};
	gen = 0;
	const _size_t RAND_SIZE = random_generator::TYPE_SIZE;
	long_number_t tmp = 1;
	tmp <<= (8 * RAND_SIZE);
	for (_size_t i = 0; i < (bytes + RAND_SIZE - 1) / RAND_SIZE; ++i) {
		gen *= tmp;
		gen += random_generator::get ();
	}
	for (_size_t i = 0; i < (RAND_SIZE - bytes % RAND_SIZE) % RAND_SIZE; ++i) {
		gen >>= 8;
	}
	return gen;
}

long_number_t gen_next_prime (const long_number_t& n) {
	long_number_t res{};
	mpz_nextprime (res.num.get_mpz_t (), n.num.get_mpz_t ());
	return res;
}

long_number_t gen_randprime (_size_t bytes, bool round) {
	long_number_t gen = gen_randint<true> (bytes);
	if (round) {
		long_number_t min = long_number_t (1) << (shift_t)(8 * bytes);
		while (gen < min) {
			gen <<= 1;
		}
	}
	return gen_next_prime (gen);
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

long_number_t pow_m (const long_number_t& base, const long_number_t& exp, const long_number_t& mod) {
	long_number_t result{};
	mpz_powm (result.num.get_mpz_t (), base.num.get_mpz_t (), exp.num.get_mpz_t (), mod.num.get_mpz_t ());
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

int probably_prime (const long_number_t& n) {
	const int TRIES = 50;
	for (int i = 15; i < TRIES; ++i) {
		int res = mpz_probab_prime_p (n.num.get_mpz_t (), i);
		if (res == 0) {
			return 0;
		}
		if (res == 2) {
			return 2;
		}
	}
	return 1;
}

long_number_t gen_safe_prime (_size_t bytes) {
	long_number_t p_ = gen_randprime (bytes);
#if UNSAFE_PRIME_ROOT
	return p_;
#else
	long_number_t p = p_ * 2 + 1;
	int res = probably_prime (p);
	while (res == 0) {
		p_ = gen_randprime (bytes);
		p = p_ * 2 + 1;
		res = probably_prime (p);
	}
	return p;
#endif
}

long_number_t get_primitive_root_prime (const long_number_t& p) {
#if UNSAFE_PRIME_ROOT
	_size_t len = (p.sizeinbase (2) + 7) / 8 + 1;
	return gen_randint<false> (len) % (p - 3) + 2;
#else
	long_number_t p_ = p >> 1;
	for (long_number_t g = 2; g < p - 1; g += 1) {
		if ((g * g) % p != 1 && pow_m (g, p_, p) != 1) {
			return g;
		}
	}
	return 3;
#endif
}

#endif
