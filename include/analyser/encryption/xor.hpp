#ifndef ENCRYPTION_XOR_H_
#define ENCRYPTION_XOR_H_

#include "../base/stream_base.hpp"
#include "../base/crypt_base.hpp"
#include <string>

class xor_cipher final : public encryption_t {
	bool sign;
	xor_cipher (bool signature);
public:
	xor_cipher () = delete;
	~xor_cipher () { }
	static const xor_cipher instance_signature;
	static const xor_cipher instance;
	_size_t encrypt (istream_base& in, ostream_base& out, const block_t& key, const block_t& precounted = block_t::empty) const override final;
	_size_t decrypt (istream_base& in, ostream_base& out, const block_t& key, const block_t& precounted = block_t::empty) const override final;
	block_t precount(const block_t &key) const override final;
};

#endif
