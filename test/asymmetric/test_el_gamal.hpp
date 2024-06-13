#ifndef TEST_ASYMMETRIC_EL_GAMAL_H_
#define TEST_ASYMMETRIC_EL_GAMAL_H_

#include "../tests_base.hpp"

#include "../../include/analyser/asymmetric/el_gamal.hpp"

res_t test_el_gamal ();

#if EL_GAMAL_TEST
ADD_TEST ("ElGamal encryption scheme", test_el_gamal);
#endif
#endif
