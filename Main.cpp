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

const _size_t BUFF_SIZE = 10000;

int main () {
	const _size_t KEY_SIZE_RSA = 2048;
	std::pair<rsa::private_key_t, rsa::public_key_t> keys = rsa::generate (KEY_SIZE_RSA);
	const int TOTAL = 100;
	int fast_bad = 0, long_bad = 0;
	for (int i = 0; i < TOTAL; ++i) {
		long_number_t ex = gen_randprime (KEY_SIZE_RSA / 8);
		long_number_t enc = keys.second.encrypt (ex);
		long_number_t dec1 = keys.first.decrypt_fast (enc), dec2 = keys.first.decrypt_long (enc);
		fast_bad += (ex != dec1);
		long_bad += (ex != dec2);
		if (i * 10 % TOTAL == 0) {
			printf ("%d\n", i * 100 / TOTAL);
		}
	}
	printf ("Fast fails: %d, long fails: %d, total: %d", fast_bad, long_bad, TOTAL);
	/*
	//AES tests
	//unsigned char cipher_key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	//unsigned char cipher_key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	//unsigned char cipher_key[] = {0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};
	unsigned char cipher_key[] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};
	char text[BUFF_SIZE] = "HELLO!!! Ok, it's too simple, but you can).";
	//unsigned char text[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
	//unsigned char text[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
	istringstream in ((char*)text, strlen (text));
	ostringstream out (aes::output_size (strlen (text)));
	_size_t result = aes::encrypt (in, out, in.buff_size (), (char*)cipher_key, aes::KEY_SIZE_3);
	printf ("Dest size: %llu\n", result);
	in.own (out.eject (), result);
	out.reload (aes::output_size (result));
	result = aes::decrypt (in, out, result, (char*)cipher_key, aes::KEY_SIZE_3);
	ibitstream in2 (out.eject (), result, true);
	bit c;
	while (in2) {
		in2 >> c;
		printf ("%d", c);
	}
	printf ("\n");
	printf ("%.*s\n", (int)strlen (text), in2.raw_view ());*/

	#ifdef __WINAPI_ENABLED__
	//SetConsoleOutputCP(CP_UTF8);//Он работает только с UTF16, не многобайтными. Так что смысла нет
	SetConsoleCP(1251);
	setlocale (LC_ALL, "Russian");
	#endif
	/*char buff[BUFF_SIZE] = {};
	printf("Enter smth: ");
	fgets(buff, BUFF_SIZE, stdin);
	obitstream __out__ ((strlen(buff) + 7) / 8);
	int cnt = 0;
	for (char *ptr = buff; *ptr != '\00'; ++ptr)
	{
		if (!(*ptr == '0' || *ptr == '1'))
			continue;
		__out__ << (*ptr == '1');
		++cnt;
	}
	printf ("%d\n", cnt);*/
	//11011111 00100001 code->decode
	//101010010000011 000100010111111 100101111011101 011010011101010 100010111111001 000100100000011 011001100111100 111101010100000 111011001100111 010000000111101 000101111000000 000111111100000 decode->code, или 001010011010111 010000010111000 000001101100111 111101010001110 001011000000001
	/*ibitstream in(__out__.eject (), (cnt + 7) / 8, true);
	_size_t encoded_size = hamming::get_decode_size (cnt, 4);
	obitstream __dest__ ((encoded_size + 7) / 8);
	hamming::decode (cnt, 4, in, __dest__);
	ibitstream in2 (__dest__.raw_view (), (encoded_size + 7) / 8);
	for (_size_t i = 0; i < encoded_size; ++i) {
		bit c;
		in2 >> c;
		printf ("%d", c);
	}
	printf ("\n");
	base_pos p {};
	p.byteoffset = 0;
	in2.seekg (p);
	_size_t decoded_size = hamming::get_encode_size (encoded_size, 4);
	obitstream __dest2__ ((decoded_size + 7) / 8);
	hamming::encode (encoded_size, 4, in2, __dest2__);
	ibitstream in3 (__dest__.eject (), __dest__.buff_size (), true);
	char* utf8 = new char[in3.buff_size () * 2 + 2];
	cp1251_to_utf8 (in3.raw_view (), utf8, in3.buff_size ());
	printf ("%s\n", utf8);
	for (_size_t i = 0; i < decoded_size; ++i) {
		bit c;
		in3 >> c;
		printf ("%d", c);
	}
	delete[] utf8;*/
	//printf ("\n%p %p %p\n", in.raw_view (), in2.raw_view (), in3.raw_view ());
	//printf ("%p %p %p\n", &in, &in2, &in3);

	//XOR test
	/*ifstream in ("/home/tim/work/Cyber/Math/task.png.encrypted");
	ofstream out ("/home/tim/work/Cyber/Math/task.png.decrypted");
	char key[] = "\x89\x50\x4e\x47";
	xor_cipher::execute_by_signature (key, sizeof (key) - 1, in, in.buff_size (), out);
	in.close ();
	out.close ();
	char key2[] = "key3232hello";
	ifstream in2 ("/home/tim/work/Cyber/Math/task.pdf.encrypted");
	ofstream out2 ("/home/tim/work/Cyber/Math/task.pdf.decrypted");
	xor_cipher::execute (key2, sizeof(key2) - 1, in2, in2.buff_size (), out2);
	in2.close ();
	out2.close ();*/


	//rle test (win1251)
	//1010 0101 0010 1100 1010 1110 1110 1110 1010 0100 1011 0111 0010 1001 0000 1000 1100 1010 1100 1000 0100 1011 1001 0001 0110 1100 1010 1101 0110 0101 0101 0110 0110
	/*obitstream __out__ ((strlen(buff) + 7) / 8);
	int cnt = 0;
	for (char *ptr = buff; *ptr != '\00'; ++ptr)
	{
		if (!(*ptr == '0' || *ptr == '1'))
			continue;
		__out__ << (*ptr == '1');
		++cnt;
	}*/
	/*ibitstream in(__out__.eject (), (cnt + 7) / 8, true);
	{
		std::pair<char*, _size_t> res = rle::decode(in, cnt);
		ibitstream is (res.first, (res.second + 7) / 8);
		//return 0;
		bit b;
		int counter = 0;
		for (_size_t i = 0; i < res.second; ++i) {
			is >> b;
			printf("%d", b);
			++counter;
			if (counter == 8) {
				counter = 0;
				printf(" ");
			}
		}
		char* result = new char[((res.second + 7) / 8 + 1) * 2];//многобайтовые строки
		cp1251_to_utf8 (res.first, result, (res.second + 7) / 8);
		printf("\n%s\n", result);
		delete[] result;
		ibitstream is2 (res.first, (res.second + 7) / 8, true);
		obitstream os2 ((cnt + 7) / 8 + 40);
		_size_t rest = rle::encode (is2, os2, res.second);
		ibitstream is3 (os2.eject (), (rest + 7) / 8, true);
		bool good = true;
		int j = 0;
		for (_size_t i = 0; i < rest; ++i) {
			is3 >> b;
			printf ("%d", b);
			while (buff[j] != '0' && buff[j] != '1') {
				++j;
			}
			good &= ((b + '0') == buff[j]);
			++j;
			if (!good) {
				printf ("\n%llu, %d: %d BAD, should be %d\n", i, j, b, buff[j - 1] - '0');
			}
		}
		printf ("\nOK:%d\n", good);
	}*/
	/*
	 /Base64 test (win1251)
	int len = strlen(buff);
	if (buff[len - 1] == '\n') {
		buff[len - 1] = '\00';
	}
#ifdef __WINAPI_ENABLED__
	char *encoded_ascii1251 = base64::encode(buff);
	char *encoded_utf8 = new char[strlen(encoded_ascii1251) * 2 + 2];
	cp1251_to_utf8(encoded_ascii1251, encoded_utf8);
	char *decoded_ascii1251 = base64::decode(encoded_ascii1251);
	char *decoded_utf8 = new char[strlen(decoded_ascii1251) * 2 + 2];
	cp1251_to_utf8(decoded_ascii1251, decoded_utf8);
#else
	char* before_encoding_ascii1251 = new char[strlen(buff) + 1];
	utf8_to_cp1251(buff, before_encoding_ascii1251);
	char* encoded_ascii1251 = base64::encode(before_encoding_ascii1251);
	char* encoded_utf8 = new char [strlen(encoded_ascii1251) * 2 + 2];
	cp1251_to_utf8(encoded_ascii1251, encoded_utf8);
	char *decoded_ascii1251 = base64::decode(encoded_ascii1251);
	char *decoded_utf8 = new char[strlen(decoded_ascii1251) * 2 + 2];
	cp1251_to_utf8(decoded_ascii1251, decoded_utf8);
	delete[] before_encoding_ascii1251;
#endif
#ifdef __WINAPI_ENABLED__
	printf("%s\n%s\n", encoded_ascii1251, decoded_ascii1251);
#else
	printf("%s\n%s\n", encoded_utf8, decoded_utf8);
#endif
	ibitstream bs(decoded_ascii1251);
	obitstream obs(strlen(decoded_ascii1251) + 2);
	int counter = 0;
	while (bs) {
		bit b;
		bs >> b;
		obs << b;
		printf("%d", b);
		++counter;
		if (counter == 8) {
			counter = 0;
			printf (" ");
		}
	}
	delete[] encoded_ascii1251;
	delete[] encoded_utf8;
	delete[] decoded_ascii1251;
	delete[] decoded_utf8;*/
}
