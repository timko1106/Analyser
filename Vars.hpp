#ifndef VARS_H
#define VARS_H

#include <cstddef>
#include <cstring>
#include <chrono>
#include <functional>
#include <fstream>
#include <cstdio>
#include <thread>

const size_t FULL = -1;

struct someinfo {
    std::thread::id __id;
    const char* file;
    const char* function;
    int line;
};

inline void hash_combine(std::size_t& seed) { }

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    hash_combine(seed, rest...);
}

#define MAKE_HASHABLE(type, ...) \
    namespace std {\
        template<> struct hash<type> {\
            std::size_t operator()(const type &t) const {\
                std::size_t ret = 0;\
                hash_combine(ret, __VA_ARGS__);\
                return ret;\
            }\
        };\
    }
MAKE_HASHABLE (someinfo, t.__id, t.file, t.function, t.line);

#define __FILE_GEN_ARGS __FILE__, __FUNCTION__, __LINE__

inline std::size_t proxy_file_name_generator (const char* file, const char* function, int line) {
    size_t __hash = std::hash<someinfo>{}({std::this_thread::get_id(), file, function, line});
    //printf ("%llu\n", __hash);
    return __hash;
}

inline void cp1251_to_utf8 (const char* in, char* out, size_t streamsize = FULL) {
    if (streamsize == FULL)streamsize = strlen(in);
    static const char table[128][4] = {
        "\2\xD0\x82","\2\xD0\x83","\3\xE2\x80\x9A","\2\xD1\x93",
        "\3\xE2\x80\x9E","\3\xE2\x80\xA6","\3\xE2\x80\xA0","\3\xE2\x80\xA1",
        "\3\xE2\x82\xAC","\3\xE2\x80\xB0","\2\xD0\x89","\3\xE2\x80\xB9",
        "\2\xD0\x8A","\2\xD0\x8C","\2\xD0\x8B","\2\xD0\x8F",
        "\2\xD1\x92","\3\xE2\x80\x98","\3\xE2\x80\x99","\3\xE2\x80\x9C",
        "\3\xE2\x80\x9D","\3\xE2\x80\xA2","\3\xE2\x80\x93","\3\xE2\x80\x94",
        "\0","\3\xE2\x84\xA2","\2\xD1\x99","\3\xE2\x80\xBA",
        "\2\xD1\x9A","\2\xD1\x9C","\2\xD1\x9B","\2\xD1\x9F",
        "\2\xC2\xA0","\2\xD0\x8E","\2\xD1\x9E","\2\xD0\x88",
        "\2\xC2\xA4","\2\xD2\x90","\2\xC2\xA6","\2\xC2\xA7",
        "\2\xD0\x81","\2\xC2\xA9","\2\xD0\x84","\2\xC2\xAB",
        "\2\xC2\xAC","\2\xC2\xAD","\2\xC2\xAE","\2\xD0\x87",
        "\2\xC2\xB0","\2\xC2\xB1","\2\xD0\x86","\2\xD1\x96",
        "\2\xD2\x91","\2\xC2\xB5","\2\xC2\xB6","\2\xC2\xB7",
        "\2\xD1\x91","\3\xE2\x84\x96","\2\xD1\x94","\2\xC2\xBB",
        "\2\xD1\x98","\2\xD0\x85","\2\xD1\x95","\2\xD1\x97",
        "\2\xD0\x90","\2\xD0\x91","\2\xD0\x92","\2\xD0\x93",
        "\2\xD0\x94","\2\xD0\x95","\2\xD0\x96","\2\xD0\x97",
        "\2\xD0\x98","\2\xD0\x99","\2\xD0\x9A","\2\xD0\x9B",
        "\2\xD0\x9C","\2\xD0\x9D","\2\xD0\x9E","\2\xD0\x9F",
        "\2\xD0\xA0","\2\xD0\xA1","\2\xD0\xA2","\2\xD0\xA3",
        "\2\xD0\xA4","\2\xD0\xA5","\2\xD0\xA6","\2\xD0\xA7",
        "\2\xD0\xA8","\2\xD0\xA9","\2\xD0\xAA","\2\xD0\xAB",
        "\2\xD0\xAC","\2\xD0\xAD","\2\xD0\xAE","\2\xD0\xAF",
        "\2\xD0\xB0","\2\xD0\xB1","\2\xD0\xB2","\2\xD0\xB3",
        "\2\xD0\xB4","\2\xD0\xB5","\2\xD0\xB6","\2\xD0\xB7",
        "\2\xD0\xB8","\2\xD0\xB9","\2\xD0\xBA","\2\xD0\xBB",
        "\2\xD0\xBC","\2\xD0\xBD","\2\xD0\xBE","\2\xD0\xBF",
        "\2\xD1\x80","\2\xD1\x81","\2\xD1\x82","\2\xD1\x83",
        "\2\xD1\x84","\2\xD1\x85","\2\xD1\x86","\2\xD1\x87",
        "\2\xD1\x88","\2\xD1\x89","\2\xD1\x8A","\2\xD1\x8B",
        "\2\xD1\x8C","\2\xD1\x8D","\2\xD1\x8E","\2\xD1\x8F"
    };
    for (int i = 0; i < streamsize; ++i) {
        if (*in & 0x80) {
            const char *p = table[(int)(0x7f & *in++)];
            if (!*p)
                continue;
            *out++ = p[1];
            if (*p == 1)
                continue;
            *out++ = p[2];
            if (*p == 2)
                continue;
            *out++ = p[3];
        }
        else
            *out++ = *in++;
    }
    *out = 0;
}

#endif
