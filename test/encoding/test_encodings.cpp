#ifndef TEST_ENCODINGS_CPP_
#define TEST_ENCODINGS_CPP_

#include "test_encodings.hpp"

res_t test_all_encodings () {
	int _all = 0, bad = 0;
	for (auto& it : encoding_t::get_supported ()) {
		MESSAGE ("Testing %s\n", it.key ().c_str ());
		const encoding_t* _alg = encoding_t::get (it.key ());
		if (_alg != it.value ()) {
			NOT_PASSED_ ("WRONG POINTER");
			++bad;
			++_all;
			continue;
		}
		istringstream in (test_message, TEST_SIZE + 1);
		ostringstream out {};
		_size_t encoded_size = _alg->encode (in, out);
		in.own (out.eject (), encoded_size);
		out.reload (0);
		_size_t decoded_size = _alg->decode (in, out);
		out << '\00';
		if (strcmp (test_message, out.raw_view ()) != 0) {
			++bad;
			NOT_PASSED ("FAILED, need: \"%s\", got: \"%.*s\"", test_message, (int)decoded_size, out.raw_view ());
			MESSAGE ("%sorigin: ", RED);
			raw_print (test_message, TEST_SIZE);
			MESSAGE_ ("\ngot: ");
			raw_print (out.raw_view (), decoded_size);
			MESSAGE ("%s\n", RESTORE);
		}
		++_all;
	}
	return {_all - bad, _all};
}



#endif 
