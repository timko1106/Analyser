#ifndef XOR_H
#define XOR_H

#include "StreamBase.hpp"

namespace xor_cipher {
    void execute (char* key, _size_t key_size, istream_base& in, _size_t streamsize, ostream_base& out) {
        for (_size_t i = 0; i < streamsize; ++i) {
            char val;
            in >> val;
            out << char(val ^ key[i % key_size]);
        }
    }
    void execute_by_signature (char* signature, _size_t signature_size, istream_base& in, _size_t streamsize, ostream_base& out) {
        char* key = new char[signature_size + 1];
        key[signature_size] = 0;
        in.read (key, signature_size);
        out.write (signature, signature_size);
        for (_size_t i = 0; i < signature_size; ++i) {
            key[i] ^= signature[i];
        }
        for (_size_t i = signature_size; i < streamsize; ++i) {
            char val;
            in >> val;
            out << char (val ^ key[i % signature_size]);
        }
        delete[] key;
    }
};

#endif
