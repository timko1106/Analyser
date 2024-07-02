#ifndef ENCODING_HAMMING_H_
#define ENCODING_HAMMING_H_

#include "../base/bitstream.hpp"
#include "../base/crypt_base.hpp"
#include <cstdio>

class hamming final : public encoding_t {
public:
	static const _size_t MIN_R = 3, MAX_R = 6, COUNT = MAX_R - MIN_R + 1;
private:
	const _size_t r;
	static wrapper<hamming> instances[COUNT];
	hamming (_size_t _r);
public:
	BLOCK_COPYING (hamming);
	~hamming ();
	_size_t encode (istream_base& in, ostream_base& out) const override final;
	_size_t decode (istream_base& in, ostream_base& out) const override final;
	static const hamming* get_instance (_size_t r);
	static void register_all ();
	_ADD(hamming);
};
_ADD_DEFINITION (hamming);

#endif
