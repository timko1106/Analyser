#ifndef ENCODING_HAMMING_CPP_
#define ENCODING_HAMMING_CPP_

#include "../../include/analyser/encoding/hamming.hpp"

static _size_t get_encode_size (_size_t streamsize, _size_t r) {
	_size_t origin_msg_block = ((_size_t)1 << r) - r - 1;
	_size_t block_size = ((_size_t)1 << r) - 1;
	return (streamsize + origin_msg_block - 1) / origin_msg_block * block_size;
}
static _size_t get_decode_size (_size_t streamsize, _size_t r) {
	_size_t origin_msg_block = ((_size_t)1 << r) - r - 1;
	_size_t block_size = ((_size_t)1 << r) - 1;
	return (streamsize + block_size - 1) / block_size * origin_msg_block;
}


hamming::hamming (_size_t _r) : encoding_t (encoding::HAMMING, \
		"Hamming-" + std::to_string (_r)), r (_r) { }
hamming::~hamming () { }
_size_t hamming::encode (istream_base& in, ostream_base& out) const {
	if (in.eof ()) {
		return 0;
	}
	_size_t src_size = get_decode_size (1, r), \
			   dest_size = get_encode_size (1, r);
	block_t buff (dest_size), parity (r);
	const _size_t BLOCK_SIZE = 1024;
	block_t raw (BLOCK_SIZE);
	obitstream os (0);
	_size_t i = 0, shift = 0;
	//В 1-нумерации все биты контроля чётности находятся на позициях - степенях 2.
	//Поэтому работает трюк: (i + 1) & i = 0 => i - бит контроля чётности в 0-нумерации.
	while (_size_t len = in.read ((char*)raw, BLOCK_SIZE)) {
		ibitstream is ((char*)raw, len, true);
		while (is) {
			if (i == 0) {
				memset ((char*)parity, 0, r);
				shift = 0;
			}
			for (; i < src_size && is; ++i) {
				bit c;
				is >> c;
				while (((shift + i + 1) & (shift + i)) == 0) {
					++shift;
				}
				buff[i + shift] = c;
			}
			if (i != src_size) {
				break;
			}
			for (i = 0; i < dest_size; ++i) {
				if ((i & (i + 1)) == 0) {
					continue;
				}
				bit c = buff[i];
				for (_size_t k = 0; k < r; ++k) {
					if ((i + 1) & (1 << k)) {
						parity[k] ^= c;
					}
				}
			}
			_size_t parity_bit = 0;
			for (_size_t k = 0; k < dest_size; ++k) {
				bit res;
				if (k & (k + 1)) {
					res = buff[k];
				} else {
					res = parity[parity_bit++];
				}
				os << res;
			}
			i = 0;
		}
		(void)is.eject ();
	}
	if (i) {
		for (; i < src_size; ++i) {
			while (((shift + i + 1) & (shift + i)) == 0) {
				++shift;
			}
			buff[i] = 0;
		}
		for (i = 0; i < dest_size; ++i) {
			if ((i & (i + 1)) == 0) {
				continue;
			}
			bit c = buff[i];
			for (_size_t k = 0; k < r; ++k) {
				if ((i + 1) & (1 << k)) {
					parity[k] ^= c;
				}
			}
		}
		_size_t parity_bit = 0;
		for (_size_t k = 0; k < dest_size; ++k) {
			bit res;
			if (k & (k + 1)) {
				res = buff[k];
			} else {
				res = parity[parity_bit++];
			}
			os << res;
		}
	}
	wrapper<base_pos> _p = os.tellg ();
	pos* p = (pos*)_p.get ();
	_size_t result = p->byteoffset + (p->bitoffset ? 1 : 0);
	out.write (os.raw_view (), result);
	return result;
}

_size_t hamming::decode (istream_base& in, ostream_base& out) const {
	if (in.eof ()) {
		return 0;
	}
	block_t b (in.buff_size ());
	_size_t useful = in.read ((char*)b, b.size ());
	ibitstream is ((const char*)b, useful);
	obitstream os (0);
	_size_t block = get_encode_size (1, r);
	block_t buff (block), errors (r);
	while (is) {
		memset ((char*)errors, 0, r);
		for (_size_t j = 0; j < block; ++j) {
			bit c;
			if (!is) {
				break;
			}
			is >> c;
			buff[j] = c;
			for (_size_t k = 0; k < r; ++k) {
				if ((j + 1) & (1 << k)) {
					errors[k] ^= c;
				}
			}
		}
		_size_t error_bit = 0;
		for (_size_t j = r; j > 0; --j) {
			error_bit <<= 1;
			error_bit |= (_size_t)errors[(_size_t)(j - 1)];
		}
		if (error_bit) {
			buff[error_bit - 1] ^= 1;
		}
		for (_size_t j = 0; j < block; ++j) {
			if (j & (j + 1)) {
				bit c = buff[j];
				os << c;
			}
		}
	}
	wrapper<base_pos> _p = os.tellg ();
	_size_t result = _p->byteoffset + (((pos*)_p.get ())->bitoffset ? 1 : 0);
	out.write (os.raw_view (), result);
	return result;
}
wrapper<hamming> hamming::instances[COUNT] = {};
const hamming* hamming::get_instance (_size_t r) {
	if (r < hamming::MIN_R || r > hamming::MAX_R) {
		return nullptr;
	}
	return hamming::instances[r - MIN_R].get ();
}
void hamming::register_all () {
	static bool init = false;
	if (!init) {
		for (_size_t r = 0; r < COUNT; ++r) {
			instances[r] = wrapper<hamming> (new hamming (r + MIN_R));
		}
		init = true;
	}
}
_ADD_LINK_(hamming, register_all, hamming);
_ADD_2(hamming, hamming);

#endif
