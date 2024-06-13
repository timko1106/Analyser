#ifndef TEST_ENCRYPTION_XOR_H_
#define TEST_ENCRYPTION_XOR_H_

#include "../tests_base.hpp"

#include "../../include/analyser/encryption/xor.hpp" 
#include "../../include/analyser/base/stringstream.hpp"
#include "../../include/analyser/base/filestream.hpp"

res_t test_xor ();

#if XOR_TEST
ADD_TEST ("XOR encryption", test_xor);
#endif
#endif
