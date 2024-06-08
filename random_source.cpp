#ifndef RANDOM_SOURCE_CPP
#define RANDOM_SOURCE_CPP

#include "random_source.hpp"

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

#endif 
