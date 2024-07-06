#ifndef TEST_ASYMMETRIC_DSA_H_
#define TEST_ASYMMETRIC_DSA_H_

#include "../../include/analyser/asymmetric/dsa.hpp"
#include "../tests_base.hpp"

res_t test_dsa ();

ADD_TEST("DSA", test_dsa);

#endif
