#ifndef ENCODING_BASE64_H_
#define ENCODING_BASE64_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../base/crypt_base.hpp"

class base64 final : public encoding_t {
	base64 ();
public:
	BLOCK_COPYING (base64);
	~base64 ();
	_size_t encode (istream_base& in, ostream_base& out) const override final;
	_size_t decode (istream_base& in, ostream_base& out) const override final;
	static const base64 instance;
};

#endif
