#ifndef TEST_ENCODING_HAMMING_H_
#define TEST_ENCODING_HAMMING_H_

#include "../tests_base.hpp"

#include "../../include/analyser/encoding/hamming.hpp"
#include "../../include/analyser/base/stringstream.hpp"

res_t test_hamming ();

#if HAMMING_TEST
ADD_TEST ("Hamming encoding", test_hamming);
#endif
#endif
