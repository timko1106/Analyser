#ifndef BASE64_H
#define BASE64_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "crypt_base.hpp"

class base64 final : public encoding_t {
	base64 ();
public:
	base64 (int) = delete;
	~base64 ();
	_size_t encode (istream_base& in, ostream_base& out) const override final;
	_size_t decode (istream_base& in, ostream_base& out) const override final;
	static const base64 instance;
};

#endif
