#ifndef HASHING_SHA256_CPP_
#define HASHING_SHA256_CPP_

#include <array>
#include <cstdio>
#include <cstdlib>

#include "endian.h"

#include "../../include/analyser/hashing/sha256.hpp"

namespace {
	constexpr unsigned PORTION = 16, GROW = 4;
	constexpr std::array<sha256::word, static_cast<std::size_t>(PORTION) * GROW> K = {
		0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
		0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
		0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
		0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
		0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
		0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
		0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
		0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
		};
	
	constexpr unsigned STATE_SIZE = 8;
	using word = sha256::word;
	using std::array;

	inline void native_to_be (_size_t& n) {
		n = htobe64 (n);
	}
	inline void native_to_be (uint32_t& n) {
		n = htobe32 (n);
	}
	inline void be_to_native (uint32_t& n) {
		n = be32toh (n);
	}

	inline uint32_t rotr (uint32_t n, uint32_t d) {
		return (n >> d) | (n << (32 - d));
	}
	inline sha256::word s0 (word n) {
		return rotr (n, 7) ^ rotr (n, 18) ^ (n >> 3);
	}
	inline sha256::word s1 (word n) {
		return rotr (n, 17) ^ rotr (n, 19) ^ (n >> 10);
	}
	inline void expand (array<word, PORTION * GROW>& w) {
		for (unsigned i = PORTION; i < GROW * PORTION; ++i) {
			w[i] = w[i - 16] + s0 (w[i - 15]) + w[i - 7] + s1 (w[i - 2]);
		}
	}

	inline word S0 (word n) {
		return rotr (n, 2) ^ rotr (n, 13) ^ rotr (n, 22);
	}
	inline sha256::word S1 (sha256::word n) {
		return rotr (n, 6) ^ rotr (n, 11) ^ rotr (n, 25);
	}
	inline word maj (word x, word y, word z) {
		return (x & y) ^ (x & z) ^ (y & z);
	}
	inline word ch (word x, word y, word z) {
		return (x & y) ^ ((~x) & z);
	}
	inline void cycle (array<word, PORTION * GROW>& w, array<word, STATE_SIZE>& state) {
		array<word, STATE_SIZE> local = state;
		for (unsigned i = 0; i < K.size (); ++i) {
			const word temp1 = local[7] + S1 (local[4]) + ch (local[4], local[5], local[6]) + K[i] + w[i];
			const word temp2 = S0(local[0]) + maj (local[0], local[1], local[2]);
			//array shift
			for (unsigned j = STATE_SIZE - 1; j > 0; --j) {
				local[j] = local[j - 1];
			}
			local[4] += temp1;
			local[0] = temp1 + temp2;
#if VERBOSE
			printf ("Iteration %u\n", i);
			for (word n : local) {
				printf ("%08x ", n);
			}
			puts ("");
#endif
		}
		for (unsigned i = 0; i < STATE_SIZE; ++i) {
			state[i] += local[i];
		}
	}

	void process (array<word, PORTION * GROW>& w, array<word, STATE_SIZE>& state) {
		expand (w);
		cycle (w, state);
	}
}  // namespace

sha256::sha256 () : hashing_t (hashes::SHA256, "SHA256", STATE_SIZE * sizeof (word)) { }

const sha256 sha256::instance {};

void sha256::hash(istream_base& in, ostream_base& out) const {
	std::array<word, STATE_SIZE> state = {
		0x6A09E667,
		0xBB67AE85,
		0x3C6EF372,
		0xA54FF53A,
		0x510E527F,
		0x9B05688C,
		0x1F83D9AB,
		0x5BE0CD19
	};
	_size_t size = 0;
	std::array<word, PORTION * GROW> w{};
	char* raw = (char*)w.data ();
	constexpr _size_t NEED_SIZE = PORTION * sizeof (word);

	bool flag_skip = false;
	while (true) {
		_size_t received = in.read (raw, NEED_SIZE);
		size += received;
		bool exit = false;
		if (received < NEED_SIZE) {
			if (!flag_skip) {
				raw[received] = (char)0x80;
			}
			//0b1000 0000
			if (flag_skip || (NEED_SIZE - (received + 1)) >= sizeof (_size_t)) {
				size <<= 3;//*8, почему-то в битах
				native_to_be (size);
				((_size_t*)raw)[PORTION / 2 - 1] = size;
				exit = true;
			} else {
				flag_skip = true;
			}
		}
#if VERBOSE
		printf ("Sent new block (flag %d)!", flag_skip);
#endif
		for (unsigned i = 0; i < PORTION; ++i) {
			be_to_native (w[i]);//Данные интерпретировать как big endian.
#if VERBOSE
			printf ("%08x", w[i]);
#endif
		}
#if VERBOSE
		puts ("");
#endif
		
		process (w, state);
		w.fill (0);
		if (exit) break;
	}
	for (word& h : state) {
		native_to_be (h);
	}
	out.write ((char*)state.data (), STATE_SIZE * sizeof (word));
}

sha256::~sha256 () { }

#endif
