#ifndef TESTS_CPP
#define TESTS_CPP

#include "Tests.hpp"

result_t test_diffie_hellan () {
	printf ("Diffie-hellman\n");
	int bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		diffie_hellman::key_t key_A = diffie_hellman::generate (KEY_SIZE_DH);
		long_number_t A = key_A.gen_A ();
		diffie_hellman::key_t key_B = diffie_hellman::key_t (key_A.get_p (), key_A.get_g (), A);
		long_number_t B = key_B.gen_A ();
		long_number_t K_B = key_B.gen_K (A), K_A = key_A.gen_K (B);
		bad += (K_A != K_B);
	}
	printf ("Bad: %d, total: %d\n", bad, TOTAL);
	return {TOTAL - bad, TOTAL};
}
result_t test_el_gamal () {
	std::pair<el_gamal::private_key_t, el_gamal::public_key_t> keys = el_gamal::generate (KEY_SIZE_EL_GAMAL);
	printf ("Generated El-Gamal\n");
	int bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		long_number_t ex = gen_randint<false> (KEY_SIZE_EL_GAMAL / 8);
		//printf ("Try:%d\nNum: %s\n", i, ex.get (10).c_str ());
		auto enc = keys.second.encode (ex);
		//printf ("Encoded: %s, %s\n", enc.first.get (10).c_str (), enc.second.get (10).c_str ());
		long_number_t dec = keys.first.decode (enc);
		//printf ("Decoded: %s\n", dec.get (10).c_str ());
		bad += (ex != dec);
	}
	printf ("Encryption tests:\nBad: %d, total: %d\n", bad, TOTAL);
	int passed = TOTAL - bad;
	bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		long_number_t m = gen_randint<false> (KEY_SIZE_EL_GAMAL / 8);
		auto signature = keys.first.sign (m);
		bad += (!keys.second.check_sign (signature, m));

	}
	printf ("Sign tests:\nBad: %d, total: %d\n", bad, TOTAL);
	passed += TOTAL - bad;
	return {passed, 2 * TOTAL};
}
result_t test_rsa () {
	std::pair<rsa::private_key_t, rsa::public_key_t> keys = rsa::generate (KEY_SIZE_RSA);
	int fast_bad = 0, long_bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		long_number_t ex = gen_randint<false> (KEY_SIZE_RSA / 8);
		//printf ("%s\n", ex.get ().c_str ());
		long_number_t enc = keys.second.encrypt (ex);
		long_number_t dec1 = keys.first.decrypt_fast (enc), dec2 = keys.first.decrypt_long (enc);
		fast_bad += (ex != dec1);
		long_bad += (ex != dec2);
	}
	printf ("Fast fails: %d, long fails: %d, total: %d\n", fast_bad, long_bad, TOTAL);
	long_number_t p = 9817, q = 9907, e = 65537, c = 36076319, d = modular_invert (e, lcm (p - 1, q - 1)), n = p * q;
	std::pair<rsa::private_key_t, rsa::public_key_t> test {rsa::private_key_t (p, q, d, n), rsa::public_key_t (n)};
	long_number_t decrypted = test.first.decrypt_fast (c);
	bool ok = decrypted == 41892906;
	std::string res = decrypted.get (10);
	printf ("Test: decoded: %s, ok:%d\n", res.c_str (), ok);
	return {2 * TOTAL - fast_bad - long_bad + ok, 2 * TOTAL + 1};
}

result_t test_aes () {
	const unsigned char cipher_key_1[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	const unsigned char cipher_key_2[] = {0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};
	const unsigned char cipher_key_3[] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};
	const unsigned char * const cipher_keys[] = {cipher_key_1, cipher_key_2, cipher_key_3};
	_size_t key_sizes[] = {aes::KEY_SIZE_1, aes::KEY_SIZE_2, aes::KEY_SIZE_3};
	int bad = 0;
	const int tests = sizeof (key_sizes) / sizeof (*key_sizes);
	for (int i = 0; i < tests; ++i) {
		const unsigned char* cipher_key = cipher_keys[i];
		const _size_t key_size = key_sizes[i];
		istringstream in (test_message);
		ostringstream out (aes::output_size (TEST_SIZE));
		_size_t result = aes::encrypt (in, out, TEST_SIZE, (const char*)cipher_key, key_size);
		in.own (out.eject (), result);
		out.reload (aes::output_size (result) + 1);
		result = aes::decrypt (in, out, result, (const char*)cipher_key, key_size);
		bad += (strcmp (test_message, out.raw_view ()) != 0);
	}
	return {tests - bad, tests};
}
result_t test_hamming () {
	_size_t rs[] = {4, 5, 6};
	const int tests = sizeof (rs) / sizeof (*rs);
	int bad = 0;
	for (int i = 0; i < tests; ++i) {
		_size_t r = rs[i];
		ibitstream in (test_message);
		_size_t encoded_size = hamming::get_encode_size (TEST_SIZE * 8, r);
		obitstream out ((encoded_size + 7) / 8);
		hamming::encode (TEST_SIZE * 8, r, in, out);
		in.own (out.eject (), (encoded_size + 7) / 8);
		_size_t decoded_size = hamming::get_decode_size (encoded_size, r);
		out.reload ((decoded_size + 7) / 8 + 1);
		hamming::decode (encoded_size, r, in, out);
		bad += (strcmp (out.raw_view (), test_message) != 0);
	}
	return {tests - bad, tests};
}
result_t test_xor () {
	const char png_signature[] = "\x89\x50\x4E\x47\x0D\x0A\x1a\x0A";
	const _size_t signature_size = sizeof (png_signature) - 1;
	const char random_key_1[] = "\x69\x17\x0A\x19\x33\xA5\x67";
	const char random_key_2[] = "\x18\x17\x0A\x18\x19\x5A";
	const _size_t key_size_1 = sizeof (random_key_1) - 1;
	const _size_t key_size_2 = sizeof (random_key_2) - 1;

	//Test1: simple text
	istringstream in (test_message);
	ostringstream out (TEST_SIZE + 1);
	xor_cipher::execute (random_key_1, key_size_1, in, TEST_SIZE, out);
	in.own (out.eject (), TEST_SIZE);
	out.reload (TEST_SIZE + 1);
	xor_cipher::execute (random_key_1, key_size_1, in, TEST_SIZE, out);
	bool _1_success = strcmp (out.raw_view(), test_message) == 0;

	//Test2: decode by key
	ifstream in2 ("xor_test2.txt");
	ofstream out2 ("xor_test2_decoded.txt");
	xor_cipher::execute (random_key_2, key_size_2, in2, in2.buff_size (), out2);
	in2.close ();
	out2.close ();
	bool _2_success = check_if_equal ("xor_test2_decoded.txt", "xor_test2_need.txt");

	//Test3: decode by signature
	ifstream in3 ("xor_test1.png");
	ofstream out3 ("xor_test1_decoded.png");
	xor_cipher::execute_by_signature (png_signature, signature_size, in3, in3.buff_size (), out3);
	in3.close ();
	out3.close ();
	bool _3_success = check_if_equal ("xor_test1_decoded.png", "xor_test1_need.png");

	return {_1_success + _2_success + _3_success, 3};

}
result_t test_rle () {
	ibitstream in (test_message);
	obitstream out (in.buff_size () * 2);
	_size_t encoded_size = rle::encode (in, out, in.buff_size () * 8);
	in.own (out.eject (), (encoded_size + 7) / 8 + 1);
	std::pair<char*, _size_t> decoded = rle::decode (in, encoded_size);
	int success = strcmp (decoded.first, test_message) == 0;
	delete[] decoded.first;
	decoded.first = nullptr;
	return {success, 1};
}
result_t test_base64 () {
	char* encoded = base64::encode (test_message);
	char* decoded = base64::decode (encoded);
	int success = strcmp (decoded, test_message) == 0;
	delete[] encoded;
	delete[] decoded;
	return {success, 1};
}
#define TEST(MACRO, func_name, name)\
	if (MACRO) {\
		++all;\
		result_t res = func_name ();\
		printf ("%s: success: %d, all: %d\n\n", name, res.first, res.second); \
		success += res.first == res.second; \
	}

result_t test_all () {
	int all = 0, success = 0;
	TEST (DIFFIE_HELLMAN_TEST, test_diffie_hellan, "Diffie-Hellman");
	TEST (EL_GAMAL_TEST, test_el_gamal, "El-Gamal");
	TEST (RSA_TEST, test_rsa, "RSA");
	TEST (AES_TEST, test_aes, "AES");
	TEST (HAMMING_TEST, test_hamming, "Hamming encoding");
	TEST (XOR_TEST, test_xor, "XOR");
	TEST (RLE_TEST, test_rle, "RLE encoding");
	TEST (BASE64_TEST, test_base64, "BASE64 encoding");
	return {success, all};
}


#endif
