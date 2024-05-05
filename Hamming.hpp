#ifndef HAMMING_H
#define HAMMING_H

#include "Bitstream.hpp"
#include <cstdio>
//streamsize всегда в битах!
namespace hamming {
    //r - количество битов контроля чётности
    size_t get_encode_size (size_t streamsize, size_t r) {
        size_t origin_msg_block = ((size_t)1 << r) - r - 1;
        size_t block_size = ((size_t)1 << r) - 1;
        return (streamsize + origin_msg_block - 1) / origin_msg_block * block_size;
    }
    size_t get_decode_size (size_t streamsize, size_t r) {
        size_t origin_msg_block = ((size_t)1 << r) - r - 1;
        size_t block_size = ((size_t)1 << r) - 1;
        return (streamsize + block_size - 1) / block_size * origin_msg_block;
    }
    void encode (size_t streamsize, size_t r, ibitstream& in, obitstream& out) {
        size_t src_size = get_decode_size (1, r);
        size_t dest_size = get_encode_size (1, r);
        char* buff = new char[dest_size];
        char* parity = new char[r];
        for (size_t i = 0; i < (streamsize + src_size - 1) / src_size; ++i) {
            memset (parity, 0, r);
#if VERBOSE
            printf ("Src: ");
#endif
            size_t shift = 0;
            for (size_t j = 0; j < src_size; ++j) {
                bit c;
                in >> c;
                while (((shift + j + 1) & (shift + j)) == 0) {
                    ++shift;
                }
                buff[j + shift] = c;
#if VERBOSE
                printf ("%d", c);
#endif
            }
#if VERBOSE
            printf ("\n");
#endif
            for (size_t j = 0; j < dest_size; ++j) {
                if ((j & (j + 1)) == 0) {
                    continue;
                }
                //bit c;
                //in >> c;
                //buff[j] = c;
                bit c = buff[j];
#if VERBOSE
                printf ("%lu:", j);
#endif
                for (size_t k = 0; k < r; ++k) {
                    if ((j + 1) & (1 << k)) {
#if VERBOSE
                        printf (" %lu,", k);
#endif
                        parity[k] ^= c;
                    }
                }
#if VERBOSE
                printf ("\n");
#endif
            }
#if VERBOSE
            printf ("\nParity bits:");
            for (size_t j = 0; j < r; ++j) {
                printf ("%d", parity[j]);
            }
            printf ("\nResult:");
#endif
            size_t parity_bit = 0;
            for (size_t k = 0; k < dest_size; ++k) {
                bit res;
                if (k & (k + 1)) {
                    res = buff[k];
                } else {
                    res = parity[parity_bit++];
                }
                out << res;
#if VERBOSE
                printf ("%d", res);
#endif
            }
#if VERBOSE
            printf ("\n");
#endif
        }
        delete[] buff;
        delete[] parity;
    }
    void decode (size_t streamsize, size_t r, ibitstream& in, obitstream& out) {
        size_t origin_block = get_decode_size (1, r);
        size_t block = get_encode_size (1, r);
        char* buff = new char[block];
        char* errors = new char[r];
        for (size_t i = 0; i < (streamsize + block - 1) / block; ++i) {
            memset (errors, 0, r);
#if VERBOSE
            printf ("Src:");
#endif
            for (size_t j = 0; j < block; ++j) {
                bit c;
                in >> c;
                buff[j] = c;
#if VERBOSE
                printf ("%d", c);
#endif
                for (size_t k = 0; k < r; ++k) {
                    if ((j + 1) & (1 << k)) {
                        errors[k] ^= c;
                    }
                }
            }
#if VERBOSE
            printf ("\n");
#endif
            size_t error_bit = 0;
            for (size_t j = r; j > 0; --j) {
                error_bit <<= 1;
                error_bit |= errors[j - 1];
            }
#if VERBOSE
            printf ("Error at %lu\nDest:", error_bit);
#endif
            if (error_bit) {
                buff[error_bit - 1] ^= 1;
            }
            for (size_t j = 0; j < block; ++j) {
                if (j & (j + 1)) {
                    bit c = buff[j];
                    out << c;
#if VERBOSE
                    printf ("%d", buff[j]);
#endif
                }
            }
#if VERBOSE
            printf ("\n");
#endif
        }
        delete[] buff;
        delete[] errors;
    }
}

#endif
