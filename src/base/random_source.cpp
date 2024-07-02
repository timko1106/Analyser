#ifndef RANDOM_SOURCE_CPP
#define RANDOM_SOURCE_CPP

#include "../../include/analyser/base/random_source.hpp"

random_generator::type random_generator::get () {
	if (iteration () % LIMIT == 0) {
		get_generator ().seed (get_random_source ()());
	}
	++iteration();
	return (unsigned)(get_generator ())();
}

random_generator::type random_generator::get_secure () {
	return get_random_source() ();
}

block_t random_generator::get_randkey (_size_t bytes) {
	block_t b (bytes);
	type* ptr = (type*)((char*)b);
	for (_size_t i = 0; i < bytes / TYPE_SIZE; ++i) {
		*ptr = get ();
		++ptr;
	}
	char* _ptr = (char*)ptr;
	for (_size_t i = 0; i < bytes % TYPE_SIZE; ++i) {
		*_ptr = (char)(get () & 0xff);
		++_ptr;
	}
	return b;
}

block_t random_generator::get_secure_randkey (_size_t bytes) {
	block_t b (bytes);
	type* ptr = (type*)((char*)b);
	for (_size_t i = 0; i < bytes / TYPE_SIZE; ++i) {
		*ptr = get_secure ();
		++ptr;
	}
	char* _ptr = (char*)ptr;
	for (_size_t i = 0; i < bytes % TYPE_SIZE; ++i) {
		*_ptr = (char)(get_secure () & 0xff);
		++_ptr;
	}
	return b;
}


#endif 
