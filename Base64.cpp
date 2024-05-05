#ifndef BASE64_CPP
#define BASE64_CPP

#include "Base64.hpp"

static const char cb64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char cd64[] = "|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

//3 8-bit -> 4 6-bit
void encode_block(unsigned char in[3], unsigned char out[4], int len) {
	out[0] = (unsigned char)cb64[(int)(in[0] >> 2)];
	out[1] = (unsigned char)cb64[(int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4))];
	out[2] = (unsigned char)(len > 1 ? cb64[(int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6))] : '=');
	out[3] = (unsigned char)(len > 2 ? cb64[(int)(in[2] & 0x3f)] : '=');
}

char* base64::encode(const char* msg, size_t size) {
	if (size == FULL)size = strlen(msg);
	size_t allocated = size * 4 / 3 + 5;
	char* result = new char[allocated];
	memset(result, 0, allocated);
	unsigned char inblock[3] = {}, outblock[4] = {};
	size_t used = 0, block_length = 0;
	char* caret = result;
	while (used < size) {
		block_length = 0;
		for (int i = 0; i < 3; ++i) {
			if (used < size) {
				inblock[i] = msg[used++];
				++block_length;
			}
			else inblock[i] = 0;
		}
		if (block_length) {
			encode_block(inblock, outblock, block_length);
			memcpy(caret, outblock, 4);
			caret += 4;
		}
	}
	return result;
}

//Decode 4 6-bit -> 3 8-bit
void decode_block(unsigned char in[4], unsigned char out[3]) {
	out[0] = (unsigned char)(in[0] << 2 | in[1] >> 4);
	out[1] = (unsigned char)(in[1] << 4 | in[2] >> 2);
	out[2] = (unsigned char)(((in[2] << 6) & 0xc0) | in[3]);
}

char* base64::decode(const char* msg, size_t size) {
	if (size == FULL)size = strlen(msg);
	size_t allocated = size * 3 / 4 + 4;
	unsigned char inblock[4] = {}, outblock[3] = {};
	char* result = new char[allocated];
	memset(result, 0, allocated);
	size_t used = 0, block_length = 0;
	char* caret = result;
	while (used < size) {
		block_length = 0;
		for (int i = 0; i < 4; ++i) {
			unsigned char v = 0;
			while (used < size && v == 0) {
				v = msg[used++];
				if (used <= size) {
					v = ((v < 43 || v > 122) ? 0 : (char)cd64[v - 43]);
					if (v != 0) {
						v = ((v == '$') ? 0 : v - 61);
					}
				}
				++block_length;
			}
			if (used <= size) {
				if (v != 0) {
					inblock[i] = (unsigned char)(v - 1);
				}
			}
			else {
				inblock[i] = (unsigned char)0;
			}
		}
		if (block_length) {
			decode_block(inblock, outblock);
            memcpy(caret, outblock, 3);
            memset(inblock, 0, 4);
			caret += 3;
		}
	}
	return result;
}


#endif
