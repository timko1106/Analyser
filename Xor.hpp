#ifndef XOR_H
#define XOR_H

#include "StreamBase.hpp"

namespace xor_cipher {
	void execute (const char* key, _size_t key_size, istream_base& in, _size_t streamsize, ostream_base& out);
	void execute_by_signature (const char* signature, _size_t signature_size, istream_base& in, _size_t streamsize, ostream_base& out);
};

#endif
