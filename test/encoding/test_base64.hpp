#ifndef TEST_ENCODING_BASE64_H_
#define TEST_ENCODING_BASE64_H_

#include "../tests_base.hpp"

#include "../../include/analyser/encoding/base64.hpp"
#include "../../include/analyser/base/stringstream.hpp"

res_t test_base64 ();

#if BASE64_TEST
ADD_TEST ("Base64 encoding", test_base64);
#endif
#endif
