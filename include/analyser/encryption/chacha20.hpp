#ifndef ENCRYPTION_SALSA20_H_
#define ENCRYPTION_SALSA20_H_

#include "../base/crypt_base.hpp"
#include <cstdint>

class chacha20 final : public encryption_t {
	chacha20 ();
	using word = uint32_t;
	static constexpr _size_t REAL_KEY = 32;
	static constexpr _size_t STATE_SIZE = 64;
	static constexpr _size_t ROW_SIZE = STATE_SIZE / 4;
	static constexpr _size_t COUNTER_SIZE = 4;
	static constexpr _size_t NONCE_SIZE = ROW_SIZE - COUNTER_SIZE;
	static constexpr word CONSTANTS[ROW_SIZE] = {0x61707865, 0x3320646e, 0x79622d32, 0x6b206574};
	static constexpr _size_t ROUNDS = 20;
	void init_block (char* ptr, const char* key) const;
	void encode_block (word* src, word* dst) const;
public:
	static constexpr _size_t KEY_SIZE = REAL_KEY + NONCE_SIZE;//bytes
	~chacha20 ();
	chacha20 (int) = delete;
	block_t precount (const block_t&) const override final;
	_size_t encrypt (istream_base& in, ostream_base& out, const block_t& key, const block_t& precounted = block_t::empty) const final override;
	_size_t decrypt (istream_base& in, ostream_base& out, const block_t& key, const block_t& precounted = block_t::empty) const final override;
	static const chacha20 instance;
};

#endif
