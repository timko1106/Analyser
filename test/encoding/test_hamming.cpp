#ifndef TEST_ENCODING_HAMMING_CPP_
#define TEST_ENCODING_HAMMING_CPP_

#include "test_hamming.hpp"

res_t test_hamming () {
	_size_t rs[] = {4, 5, 6};
	const int tests = sizeof (rs) / sizeof (*rs);
	int bad = 0;
	for (int i = 0; i < tests; ++i) {
		istringstream in (test_message, TEST_SIZE + 1);
		ostringstream out {};
		_size_t r = rs[i];
		_size_t encoded_size  = hamming::get_instance (r)->encode (in, out);
		char* buff = out.eject ();
		in.own (buff, encoded_size);
		out.reload (0);
		buff[(_size_t)rand () % encoded_size] ^= 4;
		_size_t decoded_size = encoding_t::get ("Hamming-" + std::to_string (r))->decode (in, out);
		if (raw_check_fail (out.raw_view (), decoded_size) != 0) {
			++bad;
			NOT_PASSED ("WRONG test: %d, r: %llu, need: \"%s\", got: \"%.*s\"", i + 1, r, test_message, (int)decoded_size, out.raw_view ());
			raw_bin (out.raw_view (), decoded_size);
			MESSAGE_ ("\n");
		}
	}
	return {tests - bad, tests};
}

#endif
