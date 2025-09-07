#ifndef TEST_SHA256_CPP_
#define TEST_SHA256_CPP_

#include "../../include/analyser/base/stringstream.hpp"
#include "../../include/analyser/hashing/sha256.hpp"
#include "test_sha256.hpp"

#include <cstring>

#define SEQ1(k)\
	(char)(((k) >> 24) & 0xFF), (char)(((k) >> 16) & 0xFF), (char)(((k) >> 8) & 0xFF), (char)((k) & 0xFF)
#define SEQ(k) SEQ1((uint32_t)k)

#define CONVERT(a, b, c, d, e, f, g, h)\
	SEQ(a), SEQ(b), SEQ(c), SEQ(d), SEQ(e), SEQ(f), SEQ(g), SEQ(h)
#define TEST(no)\
	++alls; \
	if (!raw_check_fail (out.raw_view (), out.used_size (), SIZE, static_cast<const char*>(TEST ## no ## _OK))) { \
		++oks; \
		printf ("Test " #no " passed (%s)!\n", TEST ## no); \
	} else { \
		printf ("Test " #no " failed (%s).\n", TEST ## no); \
	}

res_t test_sha256 () {
	constexpr char TEST1_OK[] = {
		CONVERT(0x248D6A61, 0xD20638B8, 0xE5C02693, 0x0C3E6039,
		0xA33CE459, 0x64FF2167, 0xF6ECEDD4, 0x19DB06C1)};
	char TEST1[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	istringstream in(TEST1, sizeof (TEST1) - 1, true);
	const _size_t SIZE = sha256::instance.hashed_size ();
	ostringstream out(SIZE);
	sha256::instance.hash (in, out);
	int oks = 0, alls = 0;
	TEST(1)
	
	char TEST2[] = "The quick brown fox jumps over the lazy dog";
	constexpr char TEST2_OK[] = {
		CONVERT(0xD7A8FBB3, 0x07D78094, 0x69CA9ABC, 0xB0082E4F,
		0x8D5651E4, 0x6D3CDB76, 0x2D02D0BF, 0x37C9E592)};
	(void) in.eject ();
	in.own (TEST2, sizeof (TEST2) - 1);
	out.reset ();
	sha256::instance.hash (in, out);
	TEST(2)
	
	constexpr char TEST3_OK[] = {
		CONVERT(0x8da42cf0, 0x8db5e96a, 0x775d9620, 0x2fd22673,
			0x16604e5e, 0xcc0cdb2d, 0x92ff4d60, 0xc65d3e36)};
	char TEST3[] = "ABCDEFGHIJKLMNOPQRASTUVWXYZabcdifghijklmnopqrstuvwxyz012";
	(void) in.eject ();
	in.own (TEST3, sizeof (TEST3) - 1);
	out.reset ();
	sha256::instance.hash (in, out);
	TEST(3)

	(void) in.eject ();
	return {oks, alls};
}

#endif

