#ifndef TEST_ENRYPTION_AES_H_
#define TEST_ENRYPTION_AES_H_

#include "../tests_base.hpp"

#include "../../include/analyser/encryption/aes.hpp"
#include "../../include/analyser/base/stringstream.hpp"

res_t test_aes ();

#if AES_TEST
ADD_TEST ("AES encryption", test_aes);
#endif
#endif
