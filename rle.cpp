#ifndef RLE_CPP
#define RLE_CPP

#include "rle.hpp"

rle::rle () : encoding_t (encoding::RLE, "RLE") { }
rle::~rle () { }

_size_t rle::encode (istream_base& in, ostream_base& out) const {
	if (in.eof ()) {
		return 0;
	}
	const _size_t BLOCK_SIZE = 1024;
	block_t b (BLOCK_SIZE);
	_size_t block_size = in.read ((char*)b, BLOCK_SIZE);
	ibitstream is ((char*)b, block_size, true);
	obitstream os {0};
	_size_t counter = 1;
	bit curr, next;
	is >> curr;
	os << curr;
	do {
		while (is) {
			is >> next;
			if (next == curr) {
				++counter;
				continue;
			}
			_size_t offset = (_size_t)std::log2<_size_t>(counter);
#if VERBOSE
			printf ("Series: %llu, offset: %llu\n", counter, offset);
#endif
			for (_size_t i = 0; i < offset; ++i) {
				os << (bit)(0);
			}
			for (_size_t i = 0; i <= offset; ++i) {
				os << (bit)(counter & ((_size_t)1 << (offset - i)));
			}
			curr = next;
			counter = 1;
		}
		(void)is.eject ();
	} while (block_size = in.read ((char*)b, BLOCK_SIZE), is.own ((char*)b, block_size), block_size);
	(void)is.eject ();
	if (counter) {
#if VERBOSE
		printf ("Limit: %llu\n", counter);
#endif
		_size_t offset = (_size_t)std::log2<_size_t>(counter);
#if VERBOSE
		printf ("Offset: %llu\n", offset);
#endif
		for (_size_t i = 0; i < offset; ++i) {
			os << (bit)(0);
		}
		for (_size_t i = 0; i <= offset; ++i) {
			os << (bit)(counter & ((_size_t)1 << (offset - i)));
		}
	}
	wrapper<base_pos> _p = os.tellg ();
	pos* p = (pos*)_p.get ();
	_size_t result = p->byteoffset + (p->bitoffset ? 1 : 0);
	out.write (os.raw_view (), result);
	return result;
}

_size_t rle::decode (istream_base& in, ostream_base& out) const {
	if (in.eof ()) {
		return 0;
	}
	block_t b (in.buff_size ());
	_size_t full_size = in.read ((char*)b, in.buff_size ());
	ibitstream is ((char*)b, full_size);
	obitstream os (0);
	bit curr, next;
	_size_t counter = 0;
	is >> curr;
	while (is) {
		is >> next;
		if (!next) {
			++counter;
			continue;
		}
		_size_t value = 1;
		while (counter--) {
			value <<= 1;
			is >> next;
			value |= next;
		}
		while (value--) {
			os << curr;
		}
		curr = !curr;
		counter = 0;
	}
	wrapper<base_pos> _p = os.tellg ();
	pos* p = (pos*)_p.get ();
	_size_t result = p->byteoffset + (p->bitoffset ? 1 : 0);
	out.write (os.raw_view (), result);
	return result;
}

const rle rle::instance {};

#endif
