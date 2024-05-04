#ifndef RLE_CPP
#define RLE_CPP

#include "RLE.hpp"

size_t rle::encode(ibitstream_base& in, obitstream_base& out, size_t streamsize) {
	if (streamsize == FULL)return FULL;
	bit current;
	in >> current;
	out << current;
	int counter = 1;
	size_t res{ 1 };
	for (int j = 1; j < streamsize; ++j) {
		bit next;
		in >> next;
		if (next == current) {
			++counter;
			continue;
		}
		int offset = log2(counter);
		res += 2ull * offset + 1;
		pos* p = (pos*)(out.tellg());
		for (int i = 0; i < offset; ++i) {
			out << (bit)(0);
		}
		delete p;
		for (int i = offset; i >= 0; --i) {
			out << (bit)(counter & (1 << i));
		}
		current = next;
		counter = 1;
	}
	if (counter) {
		int offset = log2(counter);
		res += 2 * offset + 1;
		pos* p = (pos*)(out.tellg());
		for (int i = 0; i < offset; ++i) {
			out << (bit)(0);
		}
		delete p;
		for (int i = offset; i >= 0; --i) {
			out << (bit)(counter & (1 << i));
		}
	}
	return res;
}

std::pair<char*, size_t> rle::decode (ibitstream_base& in, size_t streamsize) {
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
	size_t counter = 0;
	size_t size = 0;
	for (size_t i = 1; i < streamsize; ++i) {
		in >> next;
		if (!next) {
			++counter;
			continue;
		}
		size_t value = 1;
		for (int j = 0; j < counter; ++j) {
			value <<= 1;
			in >> next;
			value |= next;
			++i;
		}
		size += value;
		for (int j = 0; j < value; ++j)os << curr;
		curr = !curr;
		counter = 0;
	}
	if (bitoffset)out << curbyte;
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
