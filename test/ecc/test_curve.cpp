#ifndef ECC_TEST_CURVE_CPP_
#define ECC_TEST_CURVE_CPP_

#include "test_curve.hpp"

res_t test_ecc () {
	int _all = 0, bad = 0;
	curve ecc (long_number_t("115792089237316195423570985008687907853269984665640564039457584007908834671663", 10), long_number_t("0"), long_number_t("7"));
	point P (long_number_t ("65485170049033141755572552932091555440395722142984193594072873483228125624049", 10), long_number_t ("73163377763031141032501259779738441094247887834941211187427503803434828368457", 10));
	if (ecc.on_curve (P)) {
		OK ("Point P (%s, %s) is on curve!", P.get_x ().get (10).c_str (), P.get_y ().get (10).c_str ());
	} else {
		NOT_PASSED ("Point P (%s, %s) should be on curve", P.get_x ().get (10).c_str (), P.get_y ().get (10).c_str ());
		++bad;
	}
	++_all;
	point P2 = ecc.invert (P);
	if (ecc.on_curve (P2)) {
		OK ("Point P2 (%s, %s) is on curve!", P2.get_x ().get (10).c_str (), P2.get_y ().get (10).c_str ());
	} else {
		NOT_PASSED ("Point P2 (%s, %s) should be on curve", P2.get_x ().get (10).c_str (), P2.get_y ().get (10).c_str ());
		++bad;
	}
	++_all;
	point R = ecc.mul_2 (P);
	if (ecc.on_curve (R)) {
		OK ("Point R (%s, %s) is on curve!", R.get_x ().get (10).c_str (), R.get_y ().get (10).c_str ());
	} else {
		NOT_PASSED ("Point R (%s, %s) should be on curve", R.get_x ().get (10).c_str (), R.get_y ().get (10).c_str ());
		++bad;
	}
	++_all;
	point Rp = ecc.sum (P, P);
	if (ecc.on_curve (Rp)) {
		OK ("Point Rp (%s, %s) is on curve!", Rp.get_x ().get (10).c_str (), Rp.get_y ().get (10).c_str ());
	} else {
		NOT_PASSED ("Point Rp (%s, %s) should be on curve", Rp.get_x ().get (10).c_str (), Rp.get_y ().get (10).c_str ());
		++bad;
	}
	++_all;
	if (Rp == R) {
		OK_ ("Point R is equal to Rp");
	} else {
		NOT_PASSED_ ("Point R should be equal to Rp");
		++bad;
	}
	++_all;
	point R5 = ecc.mul (P, long_number_t ("13257", 10));
	if (ecc.on_curve (R5)){
		OK ("Point R5 (%s, %s) is on curve!", R5.get_x ().get (10).c_str (), R5.get_y ().get (10).c_str ());
	} else {
		NOT_PASSED ("Point R5 (%s, %s) should be on curve", R5.get_x ().get (10).c_str (), R5.get_y ().get (10).c_str ());
		++bad;
	}
	++_all;
	for (auto it : elliptic_curve::get_possibles ()) {
		const elliptic_curve* curve_ = it.value ().get ();
		if (curve_ == nullptr) {
			NOT_PASSED_ ("WRONG curve");
			++bad;
		} else {
			OK ("Curve %s is good!", curve_->get_name ().c_str ());
		}
		++_all;
	}
	return {_all - bad, _all};
}

#endif
