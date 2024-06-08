#ifndef VARS_CPP
#define VARS_CPP

#include "vars.hpp"

block_t::block_t (_size_t size) : ptr (new char[size]), end (ptr + size), owner (true) { }
block_t::block_t (char* p, _size_t size, bool own) : ptr (p), end (ptr + size), owner (own) { }
block_t::block_t (const char* p, _size_t size) {
	ptr = const_cast<char*> (p);
	end = ptr + size;
	owner = false;
}
block_t::block_t (const block_t& other) : \
		ptr (other.owner ? new char[other.size ()] : other.ptr), \
		end (ptr + other.size ()), owner (other.owner) { }
block_t::block_t (block_t&& other) : ptr (other.ptr), end (other.end), owner (other.owner) {
	other.ptr = nullptr;
	other.end = nullptr;
}
block_t::~block_t () {
	if (owner && ptr) {
		delete[] ptr;
	}
	ptr = nullptr;
	end = nullptr;
}
bool block_t::is_owner () const {
	return owner;
}
_size_t block_t::size () const {
	return (_size_t)(end - ptr);
}
block_t::operator char* () {
	return ptr;
}
block_t::operator const char *() const {
	return ptr;
}
char block_t::operator[] (_size_t idx) const {
	return ptr[idx];
}
char& block_t::operator[] (_size_t idx) {
	return ptr[idx];
}
block_t& block_t::operator= (block_t&& val) {
	if (owner) {
		delete[] ptr;
		ptr = nullptr;
		end = nullptr;
	}
	ptr = val.ptr;
	end = val.end;
	owner = val.owner;
	val.ptr = nullptr;
	val.end = nullptr;
	val.owner = false;
	return *this;
}
const block_t block_t::empty = block_t();

////https://github.com/Iyamoto/iconv-lite
void cp1251_to_utf8 (const char* in, char* out, _size_t streamsize) {
	if (streamsize == FULL) {
		streamsize = strlen(in);
	}
	static const int table[128] = {
		0x82D0,   0x83D0,   0x9A80E2, 0x93D1,   0x9E80E2, 0xA680E2, 0xA080E2, 0xA180E2, 0xAC82E2, 0xB080E2, 0x89D0,
		0xB980E2, 0x8AD0,   0x8CD0,   0x8BD0,   0x8FD0,   0x92D1,   0x9880E2, 0x9980E2, 0x9C80E2, 0x9D80E2, 0xA280E2,
		0x9380E2, 0x9480E2, 0,	0xA284E2, 0x99D1,   0xBA80E2, 0x9AD1,   0x9CD1,   0x9BD1,   0x9FD1,   0xA0C2,
		0x8ED0,   0x9ED1,   0x88D0,   0xA4C2,   0x90D2,   0xA6C2,   0xA7C2,   0x81D0,   0xA9C2,   0x84D0,   0xABC2,
		0xACC2,   0xADC2,   0xAEC2,   0x87D0,   0xB0C2,   0xB1C2,   0x86D0,   0x96D1,   0x91D2,   0xB5C2,   0xB6C2,
		0xB7C2,   0x91D1,   0x9684E2, 0x94D1,   0xBBC2,   0x98D1,   0x85D0,   0x95D1,   0x97D1,   0x90D0,   0x91D0,
		0x92D0,   0x93D0,   0x94D0,   0x95D0,   0x96D0,   0x97D0,   0x98D0,   0x99D0,   0x9AD0,   0x9BD0,   0x9CD0,
		0x9DD0,   0x9ED0,   0x9FD0,   0xA0D0,   0xA1D0,   0xA2D0,   0xA3D0,   0xA4D0,   0xA5D0,   0xA6D0,   0xA7D0,
		0xA8D0,   0xA9D0,   0xAAD0,   0xABD0,   0xACD0,   0xADD0,   0xAED0,   0xAFD0,   0xB0D0,   0xB1D0,   0xB2D0,
		0xB3D0,   0xB4D0,   0xB5D0,   0xB6D0,   0xB7D0,   0xB8D0,   0xB9D0,   0xBAD0,   0xBBD0,   0xBCD0,   0xBDD0,
		0xBED0,   0xBFD0,   0x80D1,   0x81D1,   0x82D1,   0x83D1,   0x84D1,   0x85D1,   0x86D1,   0x87D1,   0x88D1,
		0x89D1,   0x8AD1,   0x8BD1,   0x8CD1,   0x8DD1,   0x8ED1,   0x8FD1};
	for (_size_t i = 0; i < streamsize; ++i) {
		if (*in & 0x80) {
			int v = table[(int)(0x7f & (*(in++)))];
			if (!v) {
				continue;
			}
			*(out++) = (char)v;
			*(out++) = (char)(v >> 8);
			if (v >>= 16) {//>=2^16
				*(out++) = (char)v;
			}
		}
		else {
			*(out++) = *(in++);
		}
	}
	*out = 0;
}

struct Letter {
	char win1251;
	int	 unicode;
};

static Letter g_letters[] = {
		{(char)0x82, 0x201A},
		{(char)0x83, 0x0453},
		{(char)0x84, 0x201E},
		{(char)0x85, 0x2026},
		{(char)0x86, 0x2020},
		{(char)0x87, 0x2021},
		{(char)0x88, 0x20AC},
		{(char)0x89, 0x2030},
		{(char)0x8A, 0x0409},
		{(char)0x8B, 0x2039},
		{(char)0x8C, 0x040A},
		{(char)0x8D, 0x040C},
		{(char)0x8E, 0x040B},
		{(char)0x8F, 0x040F},
		{(char)0x90, 0x0452},
		{(char)0x91, 0x2018},
		{(char)0x92, 0x2019},
		{(char)0x93, 0x201C},
		{(char)0x94, 0x201D},
		{(char)0x95, 0x2022},
		{(char)0x96, 0x2013},
		{(char)0x97, 0x2014},
		{(char)0x99, 0x2122},
		{(char)0x9A, 0x0459},
		{(char)0x9B, 0x203A},
		{(char)0x9C, 0x045A},
		{(char)0x9D, 0x045C},
		{(char)0x9E, 0x045B},
		{(char)0x9F, 0x045F},
		{(char)0xA0, 0x00A0},
		{(char)0xA1, 0x040E},
		{(char)0xA2, 0x045E},
		{(char)0xA3, 0x0408},
		{(char)0xA4, 0x00A4},
		{(char)0xA5, 0x0490},
		{(char)0xA6, 0x00A6},
		{(char)0xA7, 0x00A7},
		{(char)0xA8, 0x0401},
		{(char)0xA9, 0x00A9},
		{(char)0xAA, 0x0404},
		{(char)0xAB, 0x00AB},
		{(char)0xAC, 0x00AC},
		{(char)0xAD, 0x00AD},
		{(char)0xAE, 0x00AE},
		{(char)0xAF, 0x0407},
		{(char)0xB0, 0x00B0},
		{(char)0xB1, 0x00B1},
		{(char)0xB2, 0x0406},
		{(char)0xB3, 0x0456},
		{(char)0xB4, 0x0491},
		{(char)0xB5, 0x00B5},
		{(char)0xB6, 0x00B6},
		{(char)0xB7, 0x00B7},
		{(char)0xB8, 0x0451},
		{(char)0xB9, 0x2116},
		{(char)0xBA, 0x0454},
		{(char)0xBB, 0x00BB},
		{(char)0xBC, 0x0458},
		{(char)0xBD, 0x0405},
		{(char)0xBE, 0x0455},
		{(char)0xBF, 0x0457}
};

void utf8_to_cp1251 (const char* utf8, char* windows1251) {
	int i = 0;
	int j = 0;
	while (utf8[i]) {
		char prefix = utf8[i];
		char suffix = utf8[i+1];
		if ((prefix & 0x80) == 0) {//0x80=0b10000000=128 (первые 128 общие, прочие - национальные)
			windows1251[j] = (char)prefix;
			++j;
		} else if ((~prefix) & 0x20) {
			int first5bit = prefix & 0x1F;
			first5bit <<= 6;
			int sec6bit = suffix & 0x3F;
			int unicode_char = first5bit + sec6bit;
			if ( unicode_char >= 0x410 && unicode_char <= 0x44F ) {
				windows1251[j] = (char)(unicode_char - 0x350);
			} else if (unicode_char >= 0x80 && unicode_char <= 0xFF) {
				windows1251[j] = (char)(unicode_char);
			} else if (unicode_char >= 0x402 && unicode_char <= 0x403) {
				windows1251[j] = (char)(unicode_char - 0x382);
			} else {
				int count = sizeof(g_letters) / sizeof(Letter);
				for (int k = 0; k < count; ++k) {
					if (unicode_char == g_letters[k].unicode) {
						windows1251[j] = g_letters[k].win1251;
						goto NEXT_LETTER;
					}
				}
				// can't convert this char
				return;
			}
NEXT_LETTER:
			++i;
			++j;
		} else {
			// can't convert this chars
			return ;
		}
		++i;
	}
	windows1251[j] = 0;
	return;
}

#endif
