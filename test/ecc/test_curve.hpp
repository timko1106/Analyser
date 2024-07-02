#ifndef ECC_TEST_CURVE_H_
#define ECC_TEST_CURVE_H_

#include "../tests_base.hpp"
#include "../../include/analyser/ecc/curve.hpp"
#include "../../include/analyser/ecc/elliptic_curve.hpp"

res_t test_ecc ();

ADD_TEST ("weierstrass elliptic curves", test_ecc);

#endif
