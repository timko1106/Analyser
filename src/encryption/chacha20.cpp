#ifndef ENCRYPTION_CHACHA20_CPP_
#define ENCRYPTION_CHACHA20_CPP_
//https://datatracker.ietf.org/doc/html/rfc7539
#include "../../include/analyser/encryption/chacha20.hpp"

#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QUARTERROUND(a, b, c, d) (             \
	a += b, d ^= a, d = ROTL(d, 16), \
	c += d, b ^= c, b = ROTL(b, 12), \
	a += b, d ^= a, d = ROTL(d,  8), \
	c += d, b ^= c, b = ROTL(b,  7))
#define QR(data, x, y, z, w) QUARTERROUND(data[x], data[y], data[z], data[w])

chacha20::chacha20 () : encryption_t (encryption::CHACHA20, "ChaCha20", true, KEY_SIZE, false) { }

block_t chacha20::precount (UNUSED const block_t& _key) const {
	return block_t::empty;
}
void chacha20::init_block (char* ptr, const char* key) const {
	memset (ptr, 0, STATE_SIZE);
	memcpy (ptr, CONSTANTS, ROW_SIZE);
	memcpy (ptr + ROW_SIZE, (const char*)key, REAL_KEY);
	//nonce будет частью ключа. counter прогоняется от 0.
	memcpy (ptr + ROW_SIZE + REAL_KEY + COUNTER_SIZE, ((const char*)key) + REAL_KEY, NONCE_SIZE);
}
void chacha20::encode_block (word* src, word* dst) const {
	for (_size_t i = 0; i < 16; ++i) {
		dst[i] = src[i];
	}
	for (_size_t i = 0; i < ROUNDS; i += 2) {
		QR(dst, 0, 4, 8, 12);
		QR(dst, 1, 5, 9, 13);
		QR(dst, 2, 6, 10, 14);
		QR(dst, 3, 7, 11, 15);
		QR(dst, 0, 5, 10, 15);
		QR(dst, 1, 6, 11, 12);
		QR(dst, 2, 7, 8, 13);
		QR(dst, 3, 4, 9, 14);
	}
	for (_size_t i = 0; i < 16; ++i) {
		dst[i] += src[i];
	}
	++(src[ROW_SIZE * 3 / 4]);
}

_size_t chacha20::encrypt (istream_base& in, ostream_base& out, const block_t& key, UNUSED const block_t& precounted) const {
	if ((const char*)key == nullptr || key.size () != KEY_SIZE) {
		return 0;
	}
	word blocks[STATE_SIZE / 4] = {}, key_stream[STATE_SIZE / 4] = {};
	init_block ((char*)blocks, (const char*)key);
	memset (key_stream, 0, STATE_SIZE);
	char* stream = (char*)key_stream;
	const _size_t BLOCKS_SIZE = 1024;
	block_t got (BLOCKS_SIZE), enc (BLOCKS_SIZE);
	_size_t keystream_idx = 0;
	_size_t len;
	_size_t res = 0;
	while ((len = in.read ((char*)got, BLOCKS_SIZE)), len) {
		for (_size_t i = 0; i < len; ++i) {
			if (keystream_idx == 0 || keystream_idx == STATE_SIZE) {
				encode_block (blocks, key_stream);
				keystream_idx = 0;
			}
			enc[i] = got[i] ^ stream[keystream_idx++];
		}
		out.write ((const char*)enc, len);
		res += len;
	}
	return res;
}

_size_t chacha20::decrypt (istream_base& in, ostream_base& out, const block_t& key, UNUSED const block_t& precounted) const {
	return encrypt (in, out, key, precounted);
}
chacha20::~chacha20 () { }
const chacha20 chacha20::instance {};

#undef ROTL
#undef QUARTERROUND
#undef QR 

#endif
