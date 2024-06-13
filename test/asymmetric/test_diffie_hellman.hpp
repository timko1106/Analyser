#ifndef TEST_ASYMMETRIC_DIFFIE_HELLMAN_H_
#define TEST_ASYMMETRIC_DIFFIE_HELLMAN_H_

#include "../tests_base.hpp"

#include "../../include/analyser/asymmetric/diffie_hellman.hpp"

res_t test_diffie_hellman ();

#if DIFFIE_HELLMAN_TEST
ADD_TEST ("Diffie-Hellman key exchange", test_diffie_hellman);
#endif
#endif
