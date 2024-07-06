#ifndef TEST_ASYMMETRIC_SIGNATURES_H_
#define TEST_ASYMMETRIC_SIGNATURES_H_

#include "../tests_base.hpp"
#include "../../include/analyser/base/crypt_base.hpp"

res_t test_all_signatures ();

ADD_TEST_PRIMARY ("all digit signatures", test_all_signatures);

#endif
