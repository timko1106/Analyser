#ifndef ENCODING_RLE_H_
#define ENCODING_RLE_H_

#include "../base/vars.hpp"
#include "../base/bitstream.hpp"
#include "../base/crypt_base.hpp"
#include <cmath>

class rle final : public encoding_t {
	rle ();
public:
	BLOCK_COPYING (rle);
	~rle ();
	_size_t encode (istream_base& in, ostream_base& out) const override final;
	_size_t decode (istream_base& in, ostream_base& out) const override final;
	static const rle instance;
};

#endif
