#ifndef BASE64_CPP
#define BASE64_CPP

#include "base64.hpp"

static const char cb64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char cd64[] = "|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

//3 8-bit -> 4 6-bit
static void encode_block(unsigned char in[3], unsigned char out[4], int len) {
	out[0] = (unsigned char)cb64[(int)(in[0] >> 2)];
	out[1] = (unsigned char)cb64[(int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4))];
	out[2] = (unsigned char)(len > 1 ? cb64[(int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6))] : '=');
	out[3] = (unsigned char)(len > 2 ? cb64[(int)(in[2] & 0x3f)] : '=');
}

//Decode 4 6-bit -> 3 8-bit
static void decode_block(unsigned char in[4], unsigned char out[3]) {
	out[0] = (unsigned char)(in[0] << 2 | in[1] >> 4);
	out[1] = (unsigned char)(in[1] << 4 | in[2] >> 2);
	out[2] = (unsigned char)(((in[2] << 6) & 0xc0) | in[3]);
}


base64::base64 () : encoding_t (encoding::BASE64, "Base64") { }
base64::~base64 () { }

_size_t base64::encode (istream_base& in, ostream_base& out) const {
	if (in.eof ()) {
		return 0;
	}
	unsigned char inblock[3] = {}, outblock[4] = {};
	int len;
	_size_t result = 0;
	while ((len = (int)in.read ((char*)inblock, 3)), len) {
		encode_block (inblock, outblock, len);
		out.write ((const char*)outblock, 4);
		result += 4;
	}
	return result;
}
_size_t base64::decode (istream_base& in, ostream_base& out) const {
	if (in.eof ()) {
		return 0;
	}
	unsigned char inblock[4] = {}, outblock[3] = {};
	_size_t result = 0;
	int len;
	while (len = (int)in.read ((char*)inblock, 4), len == 4) {
		for (int i = 0; i < len; ++i) {
			unsigned char v = inblock[i];
			v = ((v < 43 || v > 122) ? 0 : (unsigned char)cd64[v - 43]);
			if (v) {
				v = ((v == '$') ? 0 : v - 61);
			}
			inblock[i] = (v ? v - 1 : 0);
		}
		decode_block (inblock, outblock);
		out.write ((char*)outblock, 3);
		result += 3;
	}
	return result;
}
const base64 base64::instance {};

#endif
