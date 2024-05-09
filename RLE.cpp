#ifndef RLE_CPP
#define RLE_CPP

#include "RLE.hpp"

_size_t rle::encode(ibitstream_base& in, obitstream_base& out, _size_t streamsize) {
	if (streamsize == FULL)return FULL;
	bit current;
	in >> current;
	out << current;
	int counter = 1;
	_size_t res{ 1 };
#if VERBOSE
    printf ("First: %d\n", current);
#endif
	for (_size_t j = 1; j < streamsize; ++j) {
		bit next;
		in >> next;
		if (next == current) {
			++counter;
			continue;
		}
		_size_t offset = (_size_t)log2(counter);
#if VERBOSE
        printf ("Series: %d, offset: %llu\n", counter, offset);
#endif
		res += 2ull * offset + 1;
		for (_size_t i = 0; i < offset; ++i) {
			out << (bit)(0);
		}
		for (_size_t i = 0; i <= offset; ++i) {
			out << (bit)(counter & ((_size_t)1 << (offset - i)));
		}
		current = next;
		counter = 1;
	}
	if (counter) {
#if VERBOSE
        printf ("Limit: %d\n", counter);
#endif
		_size_t offset = (_size_t)log2(counter);
		res += 2 * offset + 1;
#if VERBOSE
        printf ("Offset: %llu, final length: %llu\n", offset, res);
#endif
		for (_size_t i = 0; i < offset; ++i) {
			out << (bit)(0);
		}
		for (_size_t i = 0; i <= offset; ++i) {
			out << (bit)(counter & ((_size_t)1 << (offset - i)));
		}
	}
	return res;
}

std::pair<char*, _size_t> rle::decode (ibitstream_base& in, _size_t streamsize) {
	if (streamsize == FULL) {
		return {nullptr, FULL};
	}
	std::string path = std::to_string(proxy_file_name_generator(__FILE_GEN_ARGS));
	//const char* path = proxy_file_name_generator (__FILE_GEN_ARGS).c_str();
	std::fstream out (path, std::ios_base::out);
	if (!out.is_open()) {
		return {nullptr, FULL};
	}
	unsigned char curbyte = '\00';
	int bitoffset = 0;
	auto func = [&out, &curbyte, &bitoffset](bit b){
		curbyte |= (b << (7 - bitoffset));
		++bitoffset;
		if (bitoffset == 8) {
			bitoffset = 0;
			out << (char)curbyte;
			curbyte = '\00';
		}
	};
	bit curr, next;
	obitstream_handled<decltype(func)> os (func);
	in >> curr;
	_size_t counter = 0;
	_size_t size = 0;
	for (_size_t i = 1; i < streamsize; ++i) {
		in >> next;
		if (!next) {
			++counter;
			continue;
		}
		_size_t value = 1;
		for (_size_t j = 0; j < counter; ++j) {
			value <<= 1;
			in >> next;
			value |= next;
			++i;
		}
		size += value;
		for (_size_t j = 0; j < value; ++j) {
			os << curr;
		}
		curr = !curr;
		counter = 0;
	}
	if (bitoffset) {
		out << curbyte;
	}
	out.close();
	char* buff = new char[(size + 7) / 8 + 1];//NULL-terminated
	{
		std::fstream __in__ (path, std::ios_base::in);
		__in__.read(buff, (size + 7) / 8);
		__in__.close();
	}
	std::remove (path.c_str());
	return {buff, size};
}


#endif
