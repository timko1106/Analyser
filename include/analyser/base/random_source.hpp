#ifndef BASE_RANDOM_SOURCE_H_
#define BASE_RANDOM_SOURCE_H_

#include "vars.hpp"
#include <random>

class random_generator {
	static std::random_device& get_random_source () {
		thread_local std::random_device d {};
		return d;
	}
	static std::mt19937& get_generator () {
		thread_local std::mt19937 gen{};
		return gen;
	}
	static _size_t& iteration () {
		thread_local _size_t iteration = 0;
		return iteration;
	}
	static constexpr _size_t LIMIT = 20;
public:
	random_generator () = delete;
	using type = unsigned;
	static constexpr _size_t TYPE_SIZE = sizeof (type);
	static type get ();
	static type get_secure ();
	static block_t get_randkey (_size_t bytes);
	static block_t get_secure_randkey (_size_t bytes);
};

#endif
