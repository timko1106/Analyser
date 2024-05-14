#ifndef AES_H
#define AES_H

#include "Vars.hpp"
#include "StreamBase.hpp"

#include <cstdint>

namespace aes {
	const _size_t KEY_SIZE_1 = 4;
	const _size_t KEY_SIZE_2 = 6;
	const _size_t KEY_SIZE_3 = 8;
	const _size_t ROUNDS_1 = 10;
	const _size_t ROUNDS_2 = 12;
	const _size_t ROUNDS_3 = 14;
	const _size_t KEY_SIZE = KEY_SIZE_1;//4-byte words
	const _size_t STATE_SIZE = 4;
	using byte = unsigned char;
	const _size_t WORD_SIZE = 4;
	using word = uint32_t;
	const _size_t BLOCK_SIZE = WORD_SIZE * STATE_SIZE;
    _size_t output_size (_size_t input_size);
	_size_t encrypt (istream_base& in, ostream_base& out, _size_t streamsize, char* key, _size_t key_size = KEY_SIZE);
	_size_t decrypt (istream_base& in, ostream_base& out, _size_t streamsize, char* key, _size_t key_size = KEY_SIZE);
}

#endif
