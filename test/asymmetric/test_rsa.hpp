#ifndef TEST_ASYMMETRIC_RSA_H_
#define TEST_ASYMMETRIC_RSA_H_

#include "../tests_base.hpp"

#include "../../include/analyser/asymmetric/rsa.hpp"

res_t test_rsa ();

#if RSA_TEST
ADD_TEST ("RSA public-key cryptosystem", test_rsa);
#endif
#endif
