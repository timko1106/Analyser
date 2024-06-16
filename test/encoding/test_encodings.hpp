#ifndef TEST_ENCODINGS_H_
#define TEST_ENCODINGS_H_

#include <cstring>
#include "../../include/analyser/base/crypt_base.hpp"
#include "../tests_base.hpp"
#include "../../include/analyser/base/stringstream.hpp"

res_t test_all_encodings ();

ADD_TEST_PRIMARY ("all encodings base", test_all_encodings);

#endif 
