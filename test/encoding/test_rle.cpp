#ifndef TEST_ENCODING_RLE_CPP_
#define TEST_ENCODING_RLE_CPP_ 

#include "test_rle.hpp"

res_t test_rle () {
	istringstream in (test_message, TEST_SIZE);
	ostringstream out {};
	_size_t encoded_size = rle::instance.encode (in, out);
	in.own (out.eject (), encoded_size);
	out.reload (0);
	_size_t decoded_size = encoding_t::get ("RLE")->decode (in, out);
	out << '\00';
	int success = 1;
	if (raw_check_fail (out.raw_view (), decoded_size)) {
		success = 0;
		NOT_PASSED ("WRONG need: \"%s\", got: \"%.*s\"", test_message, (int)decoded_size, out.raw_view ());
		raw_bin (out.raw_view (), decoded_size);
	}
	return {success, 1};
}

#endif
