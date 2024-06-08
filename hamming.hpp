#ifndef HAMMING_H
#define HAMMING_H

#include "bitstream.hpp"
#include "crypt_base.hpp"
#include <cstdio>

class hamming final : public encoding_t {
	_size_t r;
	static std::vector<hamming> _instances;
	static const hamming* gen_instances ();
	hamming (_size_t _r);
public:
	hamming (int) = delete;
	~hamming ();
	_size_t encode (istream_base& in, ostream_base& out) const override final;
	_size_t decode (istream_base& in, ostream_base& out) const override final;
	static const hamming* get_instance (_size_t r);
	static const _size_t MIN_R = 3, MAX_R = 10, COUNT = MAX_R - MIN_R + 1;
	static const hamming* instances;
};

#endif
