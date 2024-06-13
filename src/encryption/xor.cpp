#ifndef ENCRYPTION_XOR_CPP_
#define ENCRYPTION_XOR_CPP_

#include "../../include/analyser/encryption/xor.hpp"

xor_cipher::xor_cipher (bool signature) : encryption_t (encryption::XOR, "XOR-" + std::to_string(int (signature)), false, FULL, false), sign (signature) { }

_size_t xor_cipher::encrypt (istream_base& in, ostream_base& out, const block_t& _key, const block_t& precounted  __attribute__ ((unused))) const {
	if ((const char*)_key == nullptr || in.eof ()) {
		return 0;
	}
	const _size_t size = _key.size ();
	block_t key ((const char*)_key, size);
	_size_t result = 0;
	if (sign) {
		key = block_t (size);
		_size_t read = in.read ((char*)key, size);
		out.write ((const char*)_key, read);
		for (_size_t i = 0; i < read; ++i) {
			key[i] ^= _key[i];
		}
		result += read;
	}
	block_t tmp (size);
	_size_t len = 0;
	while (len = in.read ((char*)tmp, size), len) {
		for (_size_t i = 0; i < len; ++i) {
			tmp[i] ^= key[i];
		}
		out.write ((const char*)tmp, len);
		result += len;
	}
	return result;
}
_size_t xor_cipher::decrypt (istream_base& in, ostream_base& out, const block_t& key, const block_t& precounted) const {
	return encrypt (in, out, key, precounted);
}
block_t xor_cipher::precount (const block_t& key __attribute__ ((unused))) const {
	return block_t::empty;
}

const xor_cipher xor_cipher::instance (false);
const xor_cipher xor_cipher::instance_signature (true);

#endif
