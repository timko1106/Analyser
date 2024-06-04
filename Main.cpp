#include "Tests.hpp"

int main () {
	(void)test_all ();
	/*ifstream in ("xor_test2_need.txt");
	ofstream out ("xor_test2.txt");
	const char random_key_2[] = "\x18\x17\x0A\x18\x19\x5A";
	xor_cipher::execute (random_key_2, sizeof (random_key_2) - 1, in, in.buff_size (), out);
	in.close ();
	out.close ();*/
	/*ifstream in ("xor_test1_need.png");
	ofstream out ("xor_test1.png");
	const char random_key_1[] = "\x99\x17\x04\xAA\x59\x4F\xF0\xAB";
	xor_cipher::execute (random_key_1, sizeof (random_key_1) - 1, in, in.buff_size (), out);
	in.close ();
	out.close ();*/
	return 0;
}
