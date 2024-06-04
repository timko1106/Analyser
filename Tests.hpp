#ifndef TESTS_H
#define TESTS_H

#include "Vars.hpp"
#include "Base64.hpp"
#include "Bitstream.hpp"
#include "StringStream.hpp"
#include "RLE.hpp"
#include "FileStream.hpp"
#include "Xor.hpp"
#include "Hamming.hpp"
#include "AES.hpp"
#include "LongMath.hpp"
#include "RSA.hpp"
#include "ElGamal.hpp"
#include "DiffieHellman.hpp"
#include <cstring>

#define DIFFIE_HELLMAN_TEST 1
#define EL_GAMAL_TEST 1
#define RSA_TEST 1
#define AES_TEST 1
#define HAMMING_TEST 1
#define XOR_TEST 1
#define RLE_TEST 1
#define BASE64_TEST 1


const _size_t KEY_SIZE_DH = 2048, KEY_SIZE_EL_GAMAL = 4096, KEY_SIZE_RSA = 4096;
const int TOTAL = 10;
//const char test_message[] = "HELLO!!!";
const char test_message[] = "HELLO!!! Ok, it's not too simple, but you can).";
const _size_t TEST_SIZE = sizeof (test_message) - 1;
using result_t = std::pair<int, int>;

result_t test_diffie_hellan ();
result_t test_el_gamal ();
result_t test_rsa ();
result_t test_aes ();
result_t test_hamming ();
result_t test_xor ();
result_t test_rle ();
result_t test_base64 ();

result_t test_all ();

#endif
