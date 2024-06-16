#ifndef TEST_ENCRYPTIONS_H_
#define TEST_ENCRYPTIONS_H_

#include "../tests_base.hpp"
#include "../../include/analyser/base/crypt_base.hpp"
#include "../../include/analyser/base/stringstream.hpp"
#include "../../include/analyser/base/random_source.hpp"

res_t test_all_encryptions ();

ADD_TEST_PRIMARY ("all encryptions base", test_all_encryptions);

#endif 
