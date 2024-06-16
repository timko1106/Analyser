#ifndef TEST_ENCRYPTION_CHACHA20_H_
#define TEST_ENCRYPTION_CHACHA20_H_

#include "../tests_base.hpp"
#include "../../include/analyser/encryption/chacha20.hpp"
#include "../../include/analyser/base/crypt_base.hpp"
#include "../../include/analyser/base/stringstream.hpp"
#include "../../include/analyser/base/random_source.hpp"
#include "../../include/analyser/base/filestream.hpp"

res_t test_chacha20 ();

#if CHACHA20_TEST
ADD_TEST ("ChaCha20 encryption", test_chacha20);
#endif 

#endif
