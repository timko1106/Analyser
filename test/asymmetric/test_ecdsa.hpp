#ifndef TEST_ASYMMETRIC_ECDSA_H_
#define TEST_ASYMMETRIC_ECDSA_H_

#include "../../include/analyser/asymmetric/ecdsa.hpp"
#include "../tests_base.hpp"

res_t test_ecdsa ();

ADD_TEST("ECDSA", test_ecdsa);

#endif
