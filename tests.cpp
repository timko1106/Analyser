#ifndef TESTS_CPP
#define TESTS_CPP

#include "tests.hpp"

void tests_init () {
	std::remove ("xor_test1_decoded.png");
	std::remove ("xor_test2_decoded.txt");
	std::remove ("xor_test1.png");
	std::remove ("xor_test2.txt");
	{
		ifstream in ("xor_test2_need.txt");
		ofstream out ("xor_test2.txt");
		const char random_key_2[] = "\x18\x17\x0A\x18\x19\x5A";
		block_t key_2 (random_key_2, sizeof (random_key_2) - 1);
		xor_cipher::instance_key.encrypt (in, out, key_2);
		in.close ();
		out.close ();
	}
	{
		ifstream in ("xor_test1_need.png");
		ofstream out ("xor_test1.png");
		const char random_key_1[] = "\x99\x17\x04\xAA\x59\x4F\xF0\xAB";
		block_t key_1 (random_key_1, sizeof (random_key_1) - 1);
		encryption_t::get ("XOR-0")->encrypt (in, out, key_1);
		in.close ();
		out.close ();
	}
}

result_t test_diffie_hellan () {
	printf ("Diffie-hellman\n");
	int bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		auto key_A = diffie_hellman::instance.gen_key (KEY_SIZE_DH);
		auto A = key_A->gen_A ();
		auto key_B = asymmetric_exchange_t::get ("DiffieHellman")->gen_other (*A, *(key_A->get_public ()));
		auto B = key_B->gen_A ();
		auto K_A = key_A->gen_K (*B);
		auto K_B = key_B->gen_K (*A);
		bad += (*K_A != *K_B);
	}
	printf ("Bad: %d, total: %d\n", bad, TOTAL);
	return {TOTAL - bad, TOTAL};
}
result_t test_el_gamal () {
	auto keys = asymmetric_t::get ("ElGamal")->generate_keys (KEY_SIZE_EL_GAMAL);
	//std::pair<el_gamal::private_key_t, el_gamal::public_key_t> keys = el_gamal::generate (KEY_SIZE_EL_GAMAL);
	printf ("Generated El-Gamal\n");
	int bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		long_number_t ex = gen_randint<false> (KEY_SIZE_EL_GAMAL / 8);
		//printf ("Try:%d\nNum: %s\n", i, ex.get (10).c_str ());
		auto enc = keys.second->encrypt (ex);
		//printf ("Encoded: %s, %s\n", enc.first.get (10).c_str (), enc.second.get (10).c_str ());
		long_number_t dec = keys.first->decrypt (*enc);
		//printf ("Decoded: %s\n", dec.get (10).c_str ());
		bad += (ex != dec);
	}
	printf ("Encryption tests:\nBad: %d, total: %d\n", bad, TOTAL);
	int passed = TOTAL - bad;
	bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		long_number_t m = gen_randint<false> (KEY_SIZE_EL_GAMAL / 8);
		auto signature = keys.first->sign (m);
		bad += (!keys.second->check_sign (m, *signature));

	}
	printf ("Sign tests:\nBad: %d, total: %d\n", bad, TOTAL);
	passed += TOTAL - bad;
	const asymmetric_t* _el_gamal_ = asymmetric_t::get ("ElGamal");
	passed += (_el_gamal_ == &el_gamal_::instance);
	printf ("Checking instance: %d\n", _el_gamal_ == &el_gamal_::instance);
	return {passed, 2 * TOTAL + 1};
}
result_t test_rsa () {
	auto keys = asymmetric_t::get ("RSA")->generate_keys (KEY_SIZE_RSA);
	int fast_bad = 0, long_bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		long_number_t ex = gen_randint<false> (KEY_SIZE_RSA / 8);
		//printf ("%s\n", ex.get ().c_str ());
		auto enc = keys.second->encrypt (ex);
		long_number_t dec1 = keys.first->decrypt (*enc), dec2 = ((rsa::private_key_t*)(keys.first.get ()))->decrypt_long (*enc);
		fast_bad += (ex != dec1);
		long_bad += (ex != dec2);
	}
	printf ("Fast fails: %d, long fails: %d, total: %d\n", fast_bad, long_bad, TOTAL);
	long_number_t p = 9817, q = 9907, e = 65537, c = 36076319, d = modular_invert (e, lcm (p - 1, q - 1)), n = p * q;
	std::pair<rsa::private_key_t, rsa::public_key_t> test {rsa::private_key_t (p, q, d, n), rsa::public_key_t (n)};
	long_number_t decrypted = test.first.decrypt (rsa::encrypted_t (c));
	bool ok = decrypted == 41892906;
	std::string res = decrypted.get (10);
	printf ("Test: decoded: %s, ok:%d\n", res.c_str (), ok);
	printf ("Signature tests:\n");
	int sign_bad = 0;
	for (int i = 1; i <= TOTAL; ++i) {
		long_number_t m = gen_randint<false> (KEY_SIZE_RSA / 8);
		auto sign = keys.first->sign (m);
		sign_bad += !(keys.second->check_sign (m, *sign));
	}
	printf ("Fails: %d, total %d\n", sign_bad, TOTAL);
	return {3 * TOTAL - fast_bad - long_bad - sign_bad + ok, 3 * TOTAL + 1};
}

result_t test_aes () {
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
		bad += (strcmp (test_message, out.raw_view ()) != 0);
	}
	{
		istringstream in (test_message);
		ostringstream out {};
		block_t cipher ((const char*)cipher_keys[0], aes::KEY_SIZES[0] * aes::WORD_SIZE);
		const encryption_t* aes128 = encryption_t::get ("AES128");
		block_t precount = aes128->precount (cipher);
		_size_t result = aes128->encrypt (in, out, cipher, precount);
		in.own (out.eject (), result);
		out.reload (0);
		result = aes128->decrypt (in, out, cipher, precount);
		bad += (strcmp (test_message, out.raw_view ()) != 0);
		++tests;
	}
	return {tests - bad, tests};
}
result_t test_hamming () {
	_size_t rs[] = {4, 5, 6};
	const int tests = sizeof (rs) / sizeof (*rs);
	int bad = 0;
	for (int i = 0; i < tests; ++i) {
		istringstream in (test_message);
		ostringstream out {};
		_size_t r = rs[i];
		_size_t encoded_size  = hamming::get_instance (r)->encode (in, out);
		in.own (out.eject (), encoded_size);
		out.reload (0);
		encoding_t::get ("Hamming-" + std::to_string (r))->decode (in, out);
		bad += (strcmp (out.raw_view (), test_message) != 0);
	}
	return {tests - bad, tests};
}
result_t test_xor () {
	const char png_signature[] = "\x89\x50\x4E\x47\x0D\x0A\x1a\x0A";
	block_t signature (png_signature, sizeof (png_signature) - 1);
	const char random_key_1[] = "\x69\x17\x0A\x19\x33\xA5\x67";
	block_t key_1 (random_key_1, sizeof (random_key_1) - 1);
	const char random_key_2[] = "\x18\x17\x0A\x18\x19\x5A";
	block_t key_2 (random_key_2, sizeof (random_key_2) - 1);

	//Test1: simple text
	istringstream in (test_message);
	ostringstream out (TEST_SIZE + 1);
	xor_cipher::instance_key.encrypt (in, out, key_1);
	in.own (out.eject (), TEST_SIZE);
	out.reload (TEST_SIZE + 1);
	xor_cipher::instance_key.decrypt (in, out, key_1);
	bool _1_success = strcmp (out.raw_view(), test_message) == 0;

	//Test2: decode by key
	ifstream in2 ("xor_test2.txt");
	ofstream out2 ("xor_test2_decoded.txt");
	encryption_t::get ("XOR-0")->decrypt (in2, out2, key_2);
	in2.close ();
	out2.close ();
	bool _2_success = check_if_equal ("xor_test2_decoded.txt", "xor_test2_need.txt");

	//Test3: decode by signature
	ifstream in3 ("xor_test1.png");
	ofstream out3 ("xor_test1_decoded.png");
	//xor_cipher::instance_signature.decrypt (in3, out3, signature);
	encryption_t::get ("XOR-1")->decrypt (in3, out3, signature);
	in3.close ();
	out3.close ();
	bool _3_success = check_if_equal ("xor_test1_decoded.png", "xor_test1_need.png");

	return {_1_success + _2_success + _3_success, 3};
}
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')
result_t test_rle () {
	istringstream in (test_message, TEST_SIZE);
	ostringstream out {};
	_size_t encoded_size = rle::instance.encode (in, out);
	in.own (out.eject (), encoded_size);
	out.reload (0);
	(void)encoding_t::get ("RLE")->decode (in, out);
	out << '\00';
	int success = strcmp (out.raw_view (), test_message) == 0;
	return {success, 1};
}
result_t test_base64 () {
	istringstream in (test_message, TEST_SIZE);
	ostringstream out{};
	_size_t encoded_size = base64::instance.encode (in, out);
	printf ("%.*s\n", (int)encoded_size, out.raw_view ());
	in.own (out.eject (), encoded_size);
	out.reload (TEST_SIZE + 1);
	(void)encoding_t::get ("Base64")->decode (in, out);
	out << '\00';
	int success = strcmp (out.raw_view (), test_message) == 0;
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
	tests_init ();
	printf ("Supported encodings:\n");
	for (auto &name : encoding_t::get_supported ()) {
		printf ("%s ", name.c_str ());
	}
	printf ("\nSupported encryption:\n");
	for (auto &name : encryption_t::get_supported ()) {
		printf ("%s ", name.c_str ());
	}
	printf ("\nSupported asymmetric:\n");
	for (auto &name : asymmetric_t::get_supported ()) {
		printf ("%s ", name.c_str ());
	}
	printf ("\nSupported asymmetric key sharing:\n");
	for (auto& name : asymmetric_exchange_t::get_supported ()) {
		printf ("%s ", name.c_str ());
	}
	printf ("\n\n");
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
