#ifndef TESTS_H_
#define TESTS_H_

#include "../include/analyser/base/vars.hpp"
#include "../include/analyser/base/filestream.hpp"
#include "../include/analyser/base/crypt_base.hpp"
#include "../include/analyser/encryption/xor.hpp"
#include "tests_base.hpp"

void tests_init ();

res_t test_all ();

#endif
