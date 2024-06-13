#ifndef TEST_ENCODING_RLE_H_
#define TEST_ENCODING_RLE_H_

#include "../tests_base.hpp"

#include "../../include/analyser/encoding/rle.hpp"
#include "../../include/analyser/base/stringstream.hpp"

res_t test_rle ();

#if RLE_TEST
ADD_TEST ("RLE encoding", test_rle);
#endif
#endif
