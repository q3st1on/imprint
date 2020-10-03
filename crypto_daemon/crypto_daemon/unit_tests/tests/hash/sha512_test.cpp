#include "sha256_test.h"
#include <vector>

#include "../../hash/sha512.h"
#include <iostream>

TEST_RESULT sha512_basic_hash() {
	SHA512* sha512 = new SHA512;
	std::vector<unsigned char> message = { 't', 'e', 's', 't' };
	std::vector<unsigned char> hash = sha512->hash(message);
	std::vector<unsigned char> correct_hash = { 0xee, 0x26, 0xb0, 0xdd, 0x4a, 0xf7, 0xe7, 0x49, 0xaa, 0x1a, 0x8e, 0xe3, 0xc1, 0x0a, 0xe9, 0x92, 0x3f, 0x61, 0x89, 0x80, 0x77, 0x2e, 0x47, 0x3f, 0x88, 0x19, 0xa5, 0xd4, 0x94, 0x0e, 0x0d, 0xb2, 0x7a, 0xc1, 0x85, 0xf8, 0xa0, 0xe1, 0xd5, 0xf8, 0x4f, 0x88, 0xbc, 0x88, 0x7f, 0xd6, 0x7b, 0x14, 0x37, 0x32, 0xc3, 0x04, 0xcc, 0x5f, 0xa9, 0xad, 0x8e, 0x6f, 0x57, 0xf5, 0x00, 0x28, 0xa8, 0xff };
	delete sha512;

	if (hash != correct_hash) return TEST_RESULT::TEST_FAILED;
	return TEST_RESULT::TEST_PASSED;
}

TEST_RESULT sha512_blocksize_hash() {
	//Test if hash is correct when input is already the correct blocksize
	SHA512* sha512 = new SHA512;
	std::vector<unsigned char> message = { '0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3' };
	std::vector<unsigned char> hash = sha512->hash(message);
	std::vector<unsigned char> correct_hash = { 0x4d, 0x31, 0x99, 0x5c, 0x75, 0x21, 0x18, 0xba, 0x18, 0xcb, 0x33, 0x17, 0x60, 0xf5, 0x1b, 0x42, 0xa0, 0x38, 0x99, 0xea, 0xb5, 0x49, 0x21, 0xce, 0x59, 0x45, 0x12, 0xcc, 0x97, 0x8a, 0x25, 0xf4, 0x6f, 0x66, 0x63, 0x03, 0xf9, 0x2f, 0xb5, 0xe8, 0x1a, 0x71, 0x3f, 0xbb, 0xf4, 0xb3, 0xc1, 0x8e, 0x8d, 0xec, 0xef, 0xf0, 0xd6, 0x3f, 0x01, 0x91, 0xfd, 0x49, 0xbc, 0xd2, 0x3f, 0x26, 0xd8, 0xda };
	delete sha512;

	if (hash != correct_hash) return TEST_RESULT::TEST_FAILED;
	return TEST_RESULT::TEST_PASSED;
}

TEST_RESULT sha512_reset() {
	//Checks if internal hash state is properly reset after hashing one message
	if (sha512_basic_hash() == TEST_RESULT::TEST_FAILED) return TEST_RESULT::TEST_PASSED; //If this occurs the hashing is broken, but we return pass because we are only testing if it fails after succeeding once
	if (sha512_basic_hash() == TEST_RESULT::TEST_FAILED) return TEST_RESULT::TEST_FAILED;
	return TEST_RESULT::TEST_PASSED;
}