#include "tests.hpp"

int main () {
	auto res = test_all ();
	MESSAGE_ ("\n");
	if (res.first == res.second) {
		OK ("Total: passed all tested algorithms, count: %d", res.second);
		return 0;
	} else {
		BAD ("Total: passed %d, failed %d, all: %d", res.first, res.second - res.first, res.second);
		return 1;
	}
}
