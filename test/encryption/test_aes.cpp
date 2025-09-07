#ifndef TEST_ENCRYPTION_AES_CPP_
#define TEST_ENCRYPTION_AES_CPP_

#include "test_aes.hpp"

res_t test_aes () {
	const unsigned char cipher_key_1[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	const unsigned char cipher_key_2[] = {0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};
	const unsigned char cipher_key_3[] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};
	const unsigned char * const cipher_keys[] = {cipher_key_1, cipher_key_2, cipher_key_3};
	int bad = 0;
	int tests = aes::ALL;
	for (int i = 0; i < tests; ++i) {
		block_t cipher ((const char*)cipher_keys[i], aes::KEY_SIZES[i] * aes::WORD_SIZE);
		istringstream in (test_message, TEST_SIZE);
		ostringstream out {};
		_size_t result = aes::variants[i]->encrypt (in, out, cipher);
		in.own (out.eject (), result);
		out.reload (0);
		result = aes::variants[i]->decrypt (in, out, cipher);
		out << '\00';
		if (raw_check_fail (out.raw_view (), TEST_SIZE) != 0) {
			++bad;
			NOT_PASSED ("WRONG test: %d, key_size: %llu, need: \"%s\", got: \"%.*s\"", i + 1, cipher.size (), test_message, (int)result, out.raw_view ());
		}
	}
	const void* AES_128 = (const void*)encryption_t::get ("AES-128"), *standard = (const void*)&aes::AES128, *standard_2 = (const void*)aes::variants[0];
	if (AES_128 != standard || standard != standard_2) {
		NOT_PASSED ("Address of AES-128 (%p) differs with aes::AES128 (%p) or aes::variants[0], (%p)", AES_128, standard, standard_2);
		++bad;
	}
	++tests;
	return {tests - bad, tests};
}

#endif
