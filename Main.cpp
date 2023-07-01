#include <iostream>
#include <locale.h>
//#include "Base64.hpp"
//#include "Bitstream.hpp"
#include "RLE.hpp"

const size_t BUFF_SIZE = 10000;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define __WINAPI_ENABLED__
#else
#define UNIX
#endif

int main () {
    #ifdef __WINAPI_ENABLED__
    system("chcp 65001 >> nul");
    #endif
    setlocale(LC_ALL, "Russian");
    char buff[BUFF_SIZE] = {};
    std::cout << "Enter smth: ";
    std::cin.getline(buff, BUFF_SIZE);
    obitstream __out ((strlen(buff) + 7) / 8);
    for (char* ptr = buff; *ptr != '\00'; ++ptr)__out << (*ptr == '1');
    ibitstream in(__out.raw_view(), __out.buff_size());
    {
        std::pair<char*, size_t> res = rle::decode(in, strlen (buff));
        ibitstream is (res.first, (res.second + 7) / 8);
        //return 0;
        bit b;
        int counter = 0;
        for (int i = 0; i < res.second; ++i) {
            is >> b;
            std::cout << b;
            ++counter;
            if (counter == 8) {
                counter = 0;
                std::cout << ' ';
            }
        }
        char* result = new char[(res.second + 7) / 8];
        cp1251_to_utf8 (res.first, result, (res.second + 7) / 8);
        std::cout << '\n' << result;
        delete[] result;
        delete[] res.first;
    }
    /*

    //std::cout << out.raw_view() << '\n';
    {
        int cnt = rle::encode(in, out, strlen(buff));
        ibitstream is(out.raw_view(), cnt);
        bit b;
        int counter = 0;
        for (int i = 0; i < cnt; ++i) {
            is >> b;
            std::cout << b;
            ++counter;
            if (counter == 8) {
                counter = 0;
                std::cout << ' ';
            }
        }
    }*/
    /*
    char* encoded = base64::encode(buff);
    char* decoded = base64::decode(encoded);
    std::cout << encoded << '\n' << decoded << '\n';
    delete[] encoded;
    delete[] decoded;
    ibitstream bs(buff);
    obitstream obs(strlen(buff) + 2);
    int counter = 0;
    while (bs) {
        bit b;
        bs >> b;
        obs << b;
        std::cout << b;
        ++counter;
        if (counter == 8) {
            counter = 0;
            std::cout << ' ';
        }
    }
    pos* p = static_cast<pos*>(bs.tellg());
    --(p->byteoffset);
    bs.seekg(*p);
    delete p;
    char c;
    bs >> c;
    obs << c;
    std::cout << '\n' << obs.raw_view();*/
}
