#ifndef HAMMING_CPP
#define HAMMING_CPP

#include "Hamming.hpp"

_size_t hamming::get_encode_size (_size_t streamsize, _size_t r) {
	_size_t origin_msg_block = ((_size_t)1 << r) - r - 1;
	_size_t block_size = ((_size_t)1 << r) - 1;
	return (streamsize + origin_msg_block - 1) / origin_msg_block * block_size;
}
_size_t hamming::get_decode_size (_size_t streamsize, _size_t r) {
	_size_t origin_msg_block = ((_size_t)1 << r) - r - 1;
	_size_t block_size = ((_size_t)1 << r) - 1;
	return (streamsize + block_size - 1) / block_size * origin_msg_block;
}
void hamming::encode (_size_t streamsize, _size_t r, ibitstream& in, obitstream& out) {
	_size_t src_size = get_decode_size (1, r);
	_size_t dest_size = get_encode_size (1, r);
	char* buff = new char[dest_size];
	char* parity = new char[r];
	for (_size_t i = 0; i < (streamsize + src_size - 1) / src_size; ++i) {
		memset (parity, 0, r);
#if VERBOSE
		printf ("Src: ");
#endif
		_size_t shift = 0;
		for (_size_t j = 0; j < src_size; ++j) {
			bit c;
			in >> c;
			while (((shift + j + 1) & (shift + j)) == 0) {
				++shift;
			}
			buff[j + shift] = c;
#if VERBOSE
			printf ("%d", c);
#endif
		}
#if VERBOSE
		printf ("\n");
#endif
		for (_size_t j = 0; j < dest_size; ++j) {
			if ((j & (j + 1)) == 0) {
				continue;
			}
			bit c = buff[j];
#if VERBOSE
			printf ("%llu:", j);
#endif
			for (_size_t k = 0; k < r; ++k) {
				if ((j + 1) & (1 << k)) {
#if VERBOSE
					printf (" %llu,", k);
#endif
					parity[k] ^= c;
				}
			}
#if VERBOSE
			printf ("\n");
#endif
		}
#if VERBOSE
		printf ("\nParity bits:");
		for (_size_t j = 0; j < r; ++j) {
			printf ("%d", parity[j]);
		}
		printf ("\nResult:");
#endif
		_size_t parity_bit = 0;
		for (_size_t k = 0; k < dest_size; ++k) {
			bit res;
			if (k & (k + 1)) {
				res = buff[k];
			} else {
				res = parity[parity_bit++];
			}
			out << res;
#if VERBOSE
			printf ("%d", res);
#endif
		}
#if VERBOSE
		printf ("\n");
#endif
	}
	delete[] buff;
	delete[] parity;
}
void hamming::decode (_size_t streamsize, _size_t r, ibitstream& in, obitstream& out) {
	_size_t block = get_encode_size (1, r);
	char* buff = new char[block];
	char* errors = new char[r];
	for (_size_t i = 0; i < (streamsize + block - 1) / block; ++i) {
		memset (errors, 0, r);
#if VERBOSE
		printf ("Src:");
#endif
		for (_size_t j = 0; j < block; ++j) {
			bit c;
			in >> c;
			buff[j] = c;
#if VERBOSE
			printf ("%d", c);
#endif
			for (_size_t k = 0; k < r; ++k) {
				if ((j + 1) & (1 << k)) {
					errors[k] ^= c;
				}
			}
		}
#if VERBOSE
		printf ("\n");
#endif
		_size_t error_bit = 0;
		for (_size_t j = r; j > 0; --j) {
			error_bit <<= 1;
			error_bit |= (_size_t)errors[(_size_t)(j - 1)];
		}
#if VERBOSE
		printf ("Error at %llu\nDest:", error_bit);
#endif
		if (error_bit) {
			buff[error_bit - 1] ^= 1;
		}
		for (_size_t j = 0; j < block; ++j) {
			if (j & (j + 1)) {
				bit c = buff[j];
				out << c;
#if VERBOSE
				printf ("%d", buff[j]);
#endif
			}
		}
#if VERBOSE
		printf ("\n");
#endif
	}
	delete[] buff;
	delete[] errors;
}

#endif
