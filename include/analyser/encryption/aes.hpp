#ifndef ENCRYPTION_AES_H_
#define ENCRYPTION_AES_H_

#include "../base/vars.hpp"
#include "../base/stream_base.hpp"
#include "../base/crypt_base.hpp"

#include <cstdint>
//https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf

class aes final : public encryption_t {
	_size_t Nk, Nr;
	aes (_size_t key_size, _size_t rounds);
public:
	//Constants
	static constexpr _size_t ALL = 3;
	static constexpr _size_t KEY_SIZES[ALL] = {4, 6, 8}, ROUNDS[ALL] = {10, 12, 14};
	static const aes AES128, AES192, AES256;
	static const aes* const variants[3];
	static const _size_t STATE_SIZE = 4;
	using byte = unsigned char;
	using word = uint32_t;
	static const _size_t WORD_SIZE = 4;
	static const _size_t BLOCK_SIZE = WORD_SIZE * STATE_SIZE;


	aes () = delete;
	~aes ();
	_size_t encrypt (istream_base& in, ostream_base& out, const block_t& key, const block_t& precounted = block_t::empty) const override final;
	_size_t decrypt (istream_base& in, ostream_base& out, const block_t& key, const block_t& precounted = block_t::empty) const override final;
	block_t precount(const block_t &key) const override final;
};

#endif
