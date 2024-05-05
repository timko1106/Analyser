#include <locale.h>
#include "Base64.hpp"
#include "Bitstream.hpp"
#include "RLE.hpp"
#include "FileStream.hpp"
#include "Xor.hpp"
#include "Hamming.hpp"

const _size_t BUFF_SIZE = 10000;

int main () {
	#ifdef __WINAPI_ENABLED__
	//SetConsoleOutputCP(CP_UTF8);//Он работает только с UTF16, не многобайтными. Так что смысла нет
	SetConsoleCP(1251);
	setlocale (LC_ALL, "Russian");
	#endif
	char buff[BUFF_SIZE] = {};
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
	//11011111 00100001
	ibitstream in(__out__.raw_view (), (cnt + 7) / 8);
	_size_t encoded_size = hamming::get_encode_size (cnt, 4);
	obitstream __dest__ ((encoded_size + 7) / 8);
	hamming::encode (cnt, 4, in, __dest__);
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
	_size_t decoded_size = hamming::get_decode_size (encoded_size, 4);
	obitstream __dest2__ ((decoded_size + 7) / 8);
	hamming::decode (encoded_size, 4, in2, __dest2__);
	ibitstream in3 (__dest2__.raw_view (), __dest2__.buff_size ());
	char* utf8 = new char[in3.buff_size () * 2 + 2];
	cp1251_to_utf8 (__dest2__.raw_view (), utf8, in3.buff_size ());
	printf ("%s\n", utf8);
	for (_size_t i = 0; i < decoded_size; ++i) {
		bit c;
		in3 >> c;
		printf ("%d", c);
	}
	delete[] utf8;
	//printf ("\n%p %p %p\n", in.raw_view (), in2.raw_view (), in3.raw_view ());
	//printf ("%p %p %p\n", &in, &in2, &in3);
	/*
	//XOR test
	ifstream in ("/home/tim/work/Cyber/Math/task.png.encrypted");
	ofstream out ("/home/tim/work/Cyber/Math/task.png.decrypted");
	char key[] = "\x89\x50\x4e\x47";
	printf ("%d %d\n", in.is_open (), out.is_open ());
	xor_cipher::execute_by_signature (key, sizeof (key) - 1, in, in.buff_size (), out);
	printf ("sizeof: %lu\n", sizeof (key) - 1);*/

	/*
	//rle test (win1251)
	obitstream __out__ ((strlen(buff) + 7) / 8);
	int cnt = 0;
	for (char *ptr = buff; *ptr != '\00'; ++ptr)
	{
		if (!(*ptr == '0' || *ptr == '1'))
			continue;
		__out__ << (*ptr == '1');
		++cnt;
	}
	ibitstream in(__out__.eject(), (cnt + 7) / 8);
	{
		std::pair<char*, _size_t> res = rle::decode(in, cnt);
		ibitstream is (res.first, (res.second + 7) / 8);
		//return 0;
		bit b;
		int counter = 0;
		for (int i = 0; i < res.second; ++i) {
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
		printf("\n%s", result);
		delete[] result;
		delete[] res.first;
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
