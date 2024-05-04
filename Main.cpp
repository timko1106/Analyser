#include <locale.h>
#include "Base64.hpp"
//#include "Bitstream.hpp"
#include "RLE.hpp"
#include "FileStream.hpp"
#include "Xor.hpp"

const size_t BUFF_SIZE = 10000;

int main () {

    #ifdef __WINAPI_ENABLED__
    //SetConsoleOutputCP(CP_UTF8);//Он работает только с UTF16, не многобайтными. Так что смысла нет
    SetConsoleCP(1251);
    setlocale (LC_ALL, "Russian");
    #endif
    //XOR test
    ifstream in ("/home/tim/work/Cyber/Math/task.png.encrypted");
    ofstream out ("/home/tim/work/Cyber/Math/task.png.decrypted");
    char key[] = "\x89\x50\x4e\x47";
    printf ("%d %d\n", in.is_open (), out.is_open ());
    xor_cipher::execute_by_signature (key, sizeof (key) - 1, in, in.buff_size (), out);
    printf ("sizeof: %lu\n", sizeof (key) - 1);
    /*
    char buff[BUFF_SIZE] = {};
    printf("Enter smth: ");
    fgets(buff, BUFF_SIZE, stdin);*/
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
        std::pair<char*, size_t> res = rle::decode(in, cnt);
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
