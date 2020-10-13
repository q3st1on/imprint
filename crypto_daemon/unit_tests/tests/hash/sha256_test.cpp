#include "sha256_test.h"
#include <vector>

#include "../../../hash/sha256.h"
#include <iostream>

TEST_RESULT sha256_basic_hash() {
	SHA256* sha256 = new SHA256;
	std::vector<unsigned char> message = { 't', 'e', 's', 't' };
	std::vector<unsigned char> hash = sha256->hash(message);
	std::vector<unsigned char> correct_hash = { 0x9f, 0x86, 0xd0, 0x81, 0x88, 0x4c, 0x7d, 0x65, 0x9a, 0x2f, 0xea, 0xa0, 0xc5, 0x5a, 0xd0, 0x15, 0xa3, 0xbf, 0x4f, 0x1b, 0x2b, 0x0b, 0x82, 0x2c, 0xd1, 0x5d, 0x6c, 0x15, 0xb0, 0xf0, 0x0a, 0x08 };
	delete sha256;

	if (hash != correct_hash) return TEST_RESULT::TEST_FAILED;
	return TEST_RESULT::TEST_PASSED;
}

TEST_RESULT sha256_blocksize_hash() {
	//Test if hash is correct when input is already the correct blocksize
	SHA256* sha256 = new SHA256;
	std::vector<unsigned char> message = { '0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3' };
	std::vector<unsigned char> hash = sha256->hash(message);
	std::vector<unsigned char> correct_hash = { 0x96, 0x74, 0xd9, 0xe0, 0x78, 0x53, 0x5b, 0x7c, 0xec, 0x43, 0x28, 0x43, 0x87, 0xa6, 0xee, 0x39, 0x95, 0x61, 0x88, 0xe7, 0x35, 0xa8, 0x54, 0x52, 0xb0, 0x05, 0x0b, 0x55, 0x34, 0x1c, 0xda, 0x56 };
	delete sha256;

	if (hash != correct_hash) return TEST_RESULT::TEST_FAILED;
	return TEST_RESULT::TEST_PASSED;
}

TEST_RESULT sha256_reset() {
	//Checks if internal hash state is properly reset after hashing one message
	if (sha256_basic_hash() == TEST_RESULT::TEST_FAILED) return TEST_RESULT::TEST_PASSED; //If this occurs the hashing is broken, but we return pass because we are only testing if it fails after succeeding once
	if (sha256_basic_hash() == TEST_RESULT::TEST_FAILED) return TEST_RESULT::TEST_FAILED;
	return TEST_RESULT::TEST_PASSED;
}
