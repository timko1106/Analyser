#ifndef TEST_ENCODING_BASE64_CPP_
#define TEST_ENCODING_BASE64_CPP_

#include "test_base64.hpp"

res_t test_base64 () {
	istringstream in (test_message, TEST_SIZE);
	ostringstream out{};
	_size_t encoded_size = base64::instance.encode (in, out);
	MESSAGE ("Encoded: %.*s\n", (int)encoded_size, out.raw_view ());
	in.own (out.eject (), encoded_size);
	out.reload (0);
	_size_t decoded_size = encoding_t::get ("Base64")->decode (in, out);
	int success = !raw_check_fail (out.raw_view (), decoded_size);
	if (!success) {
		NOT_PASSED ("Test not passed: need: \"%s\", got: \"%.*s\"", test_message, (int)decoded_size, out.raw_view ());
		raw_print (out.raw_view (), decoded_size);
	}
	return {success, 1};
}

#endif
