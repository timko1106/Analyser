#ifndef RLE_H
#define RLE_H

#include "vars.hpp"
#include "bitstream.hpp"
#include "crypt_base.hpp"
#include <cmath>

class rle final : public encoding_t {
	rle ();
public:
	rle (int) = delete;
	~rle ();
	_size_t encode (istream_base& in, ostream_base& out) const override final;
	_size_t decode (istream_base& in, ostream_base& out) const override final;
	static const rle instance;
};

#endif
