#ifndef BASE_LONG_MATH_CPP_
#define BASE_LONG_MATH_CPP_

#include "../../include/analyser/base/long_math.hpp"

long_number_t::long_number_t (const char* hex_view, int base) : num (hex_view, base) { }

long_number_t::long_number_t (const std::string& s, int base) : num (s, base) { }

long_number_t::long_number_t (const long_number_t& other) : num (other.num) { }

long_number_t::long_number_t (long_number_t&& other) : num (std::forward<mpz_class>(other.num)) { }

long_number_t::long_number_t (const mpz_class& _num) : num (_num) { }

const mpz_class& long_number_t::raw () const {
	return num;
}

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
	return mpz_probab_prime_p (n.num.get_mpz_t (), 50);
}

dsa_params generation (_size_t L, _size_t N) {
	const long_number_t _L = 1 << L;
	_size_t wrong = 0;
	while (true) {
		long_number_t q = gen_randprime (N / 8);
		long_number_t q_2 = q << 1;
		for (_size_t i = 0; i < L - 1; ++i) {
			long_number_t W = (gen_randint<true>(L / 8 - 1) << 24) | gen_randint<true> (L / 8 - 1);
			long_number_t X = W + _L;
			long_number_t c = X % q_2;
			long_number_t p = X - c + 1;
			if (p < _L) {
				continue;
			}
			if (probably_prime (p) == 0) {
				//printf ("WRONG p %s\n", p.get ().c_str ());
				++wrong;
				continue;
			}
			//printf ("GOOD p %s q %s fails %llu\n", p.get ().c_str (), q.get ().c_str (), wrong);
			long_number_t e = long_number_t ((p - 1).num / q.num);
			while (true) {
				long_number_t h = gen_randint<false> ((L + 7) / 8 + 1) % (p - 3) + 2;
				long_number_t g = pow_m (h, e, p);
				if (g == 1) {
					continue;
				}
				printf ("Generated p, q, g with %llu fails\n", wrong);
				//printf ("Found g %s\n", g.get ().c_str ());
				return {p, q, g};
			}
		}
	}
}

std::pair<long_number_t, long_number_t> gen_prime_and_root (_size_t bits) {
#if UNSAFE_PRIME_ROOT
	return {gen_randprime (bytes), 2};
#else
	dsa_params p = generation (bits, bits / 2);
	return {p.p, p.g};
#endif
}

#endif
