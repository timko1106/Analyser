#ifndef ASYMMETRIC_TEST_H_
#define ASYMMETRIC_TEST_H_

#include "../tests_base.hpp"
#include "../../include/analyser/base/crypt_base.hpp"

res_t test_all_asymmetrics ();

ADD_TEST_PRIMARY ("all asymmetrics base", test_all_asymmetrics);

#endif 
