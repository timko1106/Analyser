#ifndef HASHING_SHA256_HPP_
#define HASHING_SHA256_HPP_

#include <cstdint>
#include "../base/crypt_base.hpp"

class sha256 final : public hashing_t {
	sha256 ();
public:
	using word = uint32_t;
	BLOCK_COPYING (sha256);
	~sha256 ();
	void hash (istream_base& in, ostream_base& out) const override final;
	static const sha256 instance;
};

#endif

