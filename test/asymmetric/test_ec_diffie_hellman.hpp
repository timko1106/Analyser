#ifndef TEST_ASYMMETRIC_EC_DIFFIE_HELLMAN_H_
#define TEST_ASYMMETRIC_EC_DIFFIE_HELLMAN_H_

#include "../tests_base.hpp"

#include "../../include/analyser/asymmetric/ec_diffie_hellman.hpp"

res_t test_ec_diffie_hellman ();

#if DIFFIE_HELLMAN_TEST
ADD_TEST ("Elliptic curve Diffie-Hellman key exchange", test_ec_diffie_hellman);
#endif
#endif
