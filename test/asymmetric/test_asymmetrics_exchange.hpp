#ifndef TEST_ASYMMETRIC_EXCHANGE_H_
#define TEST_ASYMMETRIC_EXCHANGE_H_

#include "../tests_base.hpp"
#include "../../include/analyser/base/crypt_base.hpp"

res_t test_all_asymmetrics_exchange ();

ADD_TEST_PRIMARY ("all asymmetric exchange common", test_all_asymmetrics_exchange);

#endif 
