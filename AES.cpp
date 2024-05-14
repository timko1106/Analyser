#ifndef AES_CPP
#define AES_CPP

#include "AES.hpp"
#include <array>

const aes::byte SBOX[256] = {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};
const aes::byte InvSBOX[256] = {
		0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
		0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
		0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
		0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
		0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
		0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
		0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
		0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
		0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
		0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
		0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
		0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
		0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
		0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
		0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
		0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};
const _size_t Nb = aes::STATE_SIZE;

_size_t aes::output_size (_size_t input_size) {
    return (input_size + aes::BLOCK_SIZE - 1) / aes::BLOCK_SIZE * aes::BLOCK_SIZE;
}

void add_round_key (aes::byte* state, aes::byte* key) {
	for (_size_t i = 0; i < Nb; ++i) {
		for (_size_t j = 0; j < aes::WORD_SIZE; ++j) {
			state[i * aes::WORD_SIZE + j] ^= *key;
			++key;
		}
	}
}
void sub_bytes (aes::byte* state) {
	for (_size_t i = 0; i < Nb; ++i) {
		for (_size_t j = 0; j < aes::WORD_SIZE; ++j) {
			*state = SBOX[*state];
			++state;
		}
	}
}
void shift_rows (aes::byte* state) {
	aes::byte tmp[aes::WORD_SIZE] = {};
	for (_size_t i = 0; i < aes::WORD_SIZE; ++i) {
		for (_size_t j = 0; j < Nb; ++j) {
			tmp[j] = state[i + aes::WORD_SIZE * j];
		}
		for (_size_t j = 0; j < Nb; ++j) {
			state[i + aes::WORD_SIZE * j] = tmp[(j + i) % 4];
		}
	}
}
constexpr aes::byte add (aes::byte a, aes::byte b) {
	return a ^ b;
}
constexpr aes::byte mul2 (aes::byte b) {
	aes::byte upper = b >> 7;
	aes::byte res = b << 1;
	res ^= (upper ? 0x1b : 0);
	return res;
}
constexpr aes::byte mul3 (aes::byte b) {
	//b*(x+1)=bx+b в виде полинома
	return add (b, mul2 (b));
}
void mix_columns (aes::byte* state) {
	aes::byte block[aes::WORD_SIZE] = {};
	for (_size_t i = 0; i < aes::WORD_SIZE; ++i) {
		//рассматриваем каждое 4-байтное слово
		//Оперируем побайтово:
		//Сложение эквивалентно исключающему или
		//Умножение происходит по модулю 11b , или в виде полинома:
		//x^8+x^4+x^3+x+1
		//На [{03},{01},{01},{02}]
		//s0'=({02}*s0)+({03}*s1)+s2+s3
		//s1'=s0+({02}*s1)+({03}*s2)+s3
		//s2'=s0+s1+({02}*s2)+({03}*s3)
		//s3'=({03}*s0)+s1+s2+({02}*s3)
		for (_size_t j = 0; j < Nb; ++j) {
			block[j] = state[i * aes::WORD_SIZE + j];
		}
		state[i * aes::WORD_SIZE] = mul2 (block[0]) ^ mul3 (block[1]) ^ block[2] ^ block[3];
		state[i * aes::WORD_SIZE + 1] = block[0] ^ mul2 (block[1]) ^ mul3 (block[2]) ^ block[3];
		state[i * aes::WORD_SIZE + 2] = block[0] ^ block[1] ^ mul2 (block[2]) ^ mul3 (block[3]);
		state[i * aes::WORD_SIZE + 3] = mul3 (block[0]) ^ block[1] ^ block[2] ^ mul2 (block[3]);
	}
}

#if VERBOSE
#define VERBOSE_PRINTING(...) \
    printf (__VA_ARGS__);\
    for (_size_t i = 0; i < aes::BLOCK_SIZE; ++i) { \
        printf ("%02x", (unsigned)state[i]); \
    } \
    printf ("\n");
#else
#define VERBOSE_PRINTING(...)
#endif

void block_encrypt (aes::byte* in, aes::byte* out, aes::byte* key, _size_t Nr) {
	aes::byte* state = out;
	memcpy (state, in, aes::BLOCK_SIZE);
    VERBOSE_PRINTING ("Input: ");
	add_round_key (state, key);
    VERBOSE_PRINTING ("Added round keys\n");
	for (_size_t i = 1; i < Nr; ++i) {
		sub_bytes (state);
        VERBOSE_PRINTING ("%llu\nAfter sub_bytes:\n", i);
		shift_rows (state);
        VERBOSE_PRINTING ("After shift:\n");
		mix_columns (state);
        VERBOSE_PRINTING ("After mixing:\n");
		add_round_key (state, key + aes::WORD_SIZE * i * Nb);
        VERBOSE_PRINTING ("Added round keys\n");
	}
	sub_bytes (state);
    VERBOSE_PRINTING ("After sub_bytes:\n");
	shift_rows (state);
    VERBOSE_PRINTING ("After shift:\n");
	add_round_key (state, key + aes::WORD_SIZE * Nr * Nb);
    VERBOSE_PRINTING ("Final\n");
}
constexpr _size_t RC_LIMIT = 40;
constexpr std::array<aes::byte, RC_LIMIT> rc () {
	std::array<aes::byte, RC_LIMIT> arr{};
	arr[1] = 1;
	for (_size_t i = 2; i < RC_LIMIT; ++i) {
		arr[i] = mul2 (arr[i - 1]);
	}
	return arr;
}
aes::word rcon (aes::byte i) {
	return ((aes::word)rc ()[i]) << 24;
}
aes::word sub_word (aes::word w) {
	aes::byte* ptr = (aes::byte*)&w;
	for (_size_t i = 0; i < aes::WORD_SIZE; ++i) {
		*ptr = SBOX[*ptr];
		++ptr;
	}
	return w;
}
aes::word rot_word (aes::word w) {
	aes::word leading = w >> 24;
	return (w << 8) | leading;
}
char to_hex (aes::byte b) {
	return b >= 10 ? 'a' + (b - 10) : '0' + b;
}
aes::word get (aes::byte* b) {
	return ((b[0] * 256 + b[1]) * 256 + b[2]) * 256 + b[3];
}
void set (aes::byte* dest, aes::word what) {
	dest[0] = (aes::byte)(what >> 24);
	dest[1] = (aes::byte)((what >> 16) & 0xFF);
	dest[2] = (aes::byte)((what >> 8) & 0xFF);
	dest[3] = (aes::byte)(what & 0xFF);
}
void key_expansion (aes::byte* key, _size_t Nk, aes::byte* rounds, _size_t Nr) {
	aes::word prev;
	_size_t i = 0;
	aes::byte* ptr = rounds;
	for (; i < Nk; ++i) {
		set (ptr + i * aes::WORD_SIZE, get (key + i * aes::WORD_SIZE));
	}
	for (; i < Nb * (Nr + 1); ++i) {
		prev = get (rounds + (i - 1) * aes::WORD_SIZE);
		if (i % Nk == 0) {
			set (rounds + i * aes::WORD_SIZE, sub_word (rot_word (prev)) ^  get (rounds + (i - Nk) * aes::WORD_SIZE) ^ rcon (i / Nk));
		} else if (Nk > 6 && i % Nk == 4) {
			set (rounds + i * aes::WORD_SIZE, get (rounds + (i - Nk) * aes::WORD_SIZE) ^ sub_word (prev));
		} else {
			set (rounds + i * aes::WORD_SIZE, get (rounds + (i - Nk) * aes::WORD_SIZE) ^ prev);
		}
	}
#if VERBOSE
	printf ("Key:");
	ptr = rounds;
	for (_size_t i = 0; i < aes::WORD_SIZE * Nb * (Nr + 1); ++i) {
		printf ("%c%c", to_hex((*ptr) / 16), to_hex ((*ptr) % 16));
		++ptr;
	}
	printf ("\n");
#endif
}

_size_t aes::encrypt (istream_base& in, ostream_base& out, _size_t streamsize, char* key, _size_t key_size) {
	_size_t Nk = key_size, Nr;
	switch (Nk) {
		case KEY_SIZE_1:
			Nr = ROUNDS_1;
			break;
		case KEY_SIZE_2:
			Nr = ROUNDS_2;
			break;
		case KEY_SIZE_3:
			Nr = ROUNDS_3;
			break;
		default:
			return 0;
	}
#if VERBOSE
	printf ("Nk: %llu, Nr: %llu, streamsize: %llu, result: %llu\n", Nk, Nr, streamsize, (streamsize + aes::BLOCK_SIZE - 1) / aes::BLOCK_SIZE * aes::BLOCK_SIZE);
#endif
	byte* rounds = new byte[WORD_SIZE * Nb * (Nr + 1)];
	aes::byte in_block[aes::BLOCK_SIZE] {}, out_block[aes::BLOCK_SIZE] {};
	key_expansion ((byte*)key, Nk, rounds, Nr);
	for (_size_t i = 0; i < (streamsize + aes::BLOCK_SIZE - 1) / aes::BLOCK_SIZE; ++i) {
		in.read ((char*)in_block, aes::BLOCK_SIZE);
		block_encrypt (in_block, out_block, rounds, Nr);
		out.write ((char*)out_block, aes::BLOCK_SIZE);
	}
	delete[] rounds;
	return (streamsize + aes::BLOCK_SIZE - 1) / aes::BLOCK_SIZE * aes::BLOCK_SIZE;
}
void inv_sub_bytes (aes::byte* state) {
	for (_size_t i = 0; i < Nb; ++i) {
		for (_size_t j = 0; j < aes::WORD_SIZE; ++j) {
			*state = InvSBOX[*state];
			++state;
		}
	}
}
void inv_shift_rows (aes::byte* state) {
	aes::byte tmp[aes::WORD_SIZE] = {};
	for (_size_t i = 0; i < aes::WORD_SIZE; ++i) {
		for (_size_t j = 0; j < Nb; ++j) {
			tmp[j] = state[i + aes::WORD_SIZE * j];
		}
		for (_size_t j = 0; j < Nb; ++j) {
			state[i + aes::WORD_SIZE * j] = tmp[(j - i + aes::WORD_SIZE) % aes::WORD_SIZE];
		}
	}
}

constexpr aes::byte mul (aes::byte a, aes::byte b) {
	aes::byte res = 0;
	while (a && b) {
		if (b & 1) {
			res = add (res, a);
		}
		a = mul2 (a);
		b >>= 1;
		//a * b = (a * 2) * (b / 2) + a * (b % 2)
	}
	return res;
}

void inv_mix_columns (aes::byte* state) {
	aes::byte block[aes::WORD_SIZE] = {};
	for (_size_t i = 0; i < aes::WORD_SIZE; ++i) {
        //[{0b},{0d},{09},{0e}]
		//s0'=({0e}*s0)+({0b}*s1)+({0d}*s2)+({09}*s3)
		//s1'=({0e}*s1)+({0b}*s2)+({0d}*s3)+({09}*s0)
		//s1'=({0e}*s2)+({0b}*s3)+({0d}*s0)+({09}*s1)
		//s1'=({0e}*s3)+({0b}*s0)+({0d}*s1)+({09}*s2)
		for (_size_t j = 0; j < Nb; ++j) {
			block[j] = state[i * aes::WORD_SIZE + j];
		}
		state[i * aes::WORD_SIZE] = mul (block[0], 0x0e) ^ mul (block[1], 0x0b) ^ mul (block[2], 0x0d) ^ mul (block[3], 0x09);
		state[i * aes::WORD_SIZE + 1] = mul (block[1], 0x0e) ^ mul (block[2], 0x0b) ^ mul (block[3], 0x0d) ^ mul (block[0], 0x09);
		state[i * aes::WORD_SIZE + 2] = mul (block[2], 0x0e) ^ mul (block[3], 0x0b) ^ mul (block[0], 0x0d) ^ mul (block[1], 0x09);
		state[i * aes::WORD_SIZE + 3] = mul (block[3], 0x0e) ^ mul (block[0], 0x0b) ^ mul (block[1], 0x0d) ^ mul (block[2], 0x09);
	}
}

void block_decrypt (aes::byte* in, aes::byte* out, aes::byte* key, _size_t Nr) {
	aes::byte* state = out;
	memcpy (state, in, aes::BLOCK_SIZE);
    VERBOSE_PRINTING ("Input: ");
	add_round_key (state, key + aes::WORD_SIZE * Nr * Nb);
    VERBOSE_PRINTING ("Added round key:\n");
	for (_size_t i = Nr - 1; i >= 1; --i) {
        inv_sub_bytes (state);
        VERBOSE_PRINTING ("%llu\nAfter inv_sub_bytes:\n", i);
		inv_shift_rows (state);
        VERBOSE_PRINTING ("After inv_shift_rows:\n");
		inv_mix_columns (state);
        VERBOSE_PRINTING ("After inv_mix_columns:\n");
        add_round_key (state, key + aes::WORD_SIZE * i * Nb);
        VERBOSE_PRINTING ("Added round key:\n");
	}
	inv_sub_bytes (state);
    VERBOSE_PRINTING ("After inv_sub_bytes:\n");
	inv_shift_rows (state);
    VERBOSE_PRINTING ("After inv_shift_rows:\n");
	add_round_key (state, key);
    VERBOSE_PRINTING ("Final:\n");
}
#undef VERBOSE_PRINTING
void key_expansion_decrypt (aes::byte* key, _size_t Nk, aes::byte* rounds, _size_t Nr) {
    key_expansion (key, Nk, rounds, Nr);
    for (_size_t i = 1; i <= Nr - 1; ++i) {
        inv_mix_columns (rounds + aes::WORD_SIZE * i * Nb);
    }
#if VERBOSE
    printf ("Key:");
    for (_size_t i = 0; i < aes::WORD_SIZE * Nb * (Nr + 1); ++i) {
        printf ("%02x", (unsigned)rounds[i]);
    }
    printf ("\n");
#endif
}
_size_t aes::decrypt (istream_base& in, ostream_base& out, _size_t streamsize, char* key, _size_t key_size) {
    _size_t Nk = key_size, Nr;
	switch (Nk) {
		case KEY_SIZE_1:
			Nr = ROUNDS_1;
			break;
		case KEY_SIZE_2:
			Nr = ROUNDS_2;
			break;
		case KEY_SIZE_3:
			Nr = ROUNDS_3;
			break;
		default:
			return 0;
	}
#if VERBOSE
	printf ("Nk: %llu, Nr: %llu, streamsize: %llu, result: %llu\n", Nk, Nr, streamsize, (streamsize + aes::BLOCK_SIZE - 1) / aes::BLOCK_SIZE * aes::BLOCK_SIZE);
#endif
	byte* rounds = new byte[WORD_SIZE * Nb * (Nr + 1)];
	aes::byte in_block[aes::BLOCK_SIZE] {}, out_block[aes::BLOCK_SIZE] {};
	key_expansion_decrypt ((byte*)key, Nk, rounds, Nr);
	for (_size_t i = 0; i < (streamsize + aes::BLOCK_SIZE - 1) / aes::BLOCK_SIZE; ++i) {
		in.read ((char*)in_block, aes::BLOCK_SIZE);
		block_decrypt (in_block, out_block, rounds, Nr);
		out.write ((char*)out_block, aes::BLOCK_SIZE);
	}
	delete[] rounds;
	return (streamsize + aes::BLOCK_SIZE - 1) / aes::BLOCK_SIZE * aes::BLOCK_SIZE;

}


#endif
