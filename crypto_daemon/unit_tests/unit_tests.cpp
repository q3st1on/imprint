#include "unit_tests.h"
#include <iostream>
#include <chrono>

#include "tests/bigint_test.h"
#include "tests/hash/sha256_test.h"
#include "tests/hash/sha512_test.h"

std::vector<UnitTest*> UnitTests::m_tests;

void UnitTests::load_tests() {
	//BigInt tests
	add_test(new UnitTest("bigint", "string_parsing", string_parsing));
	add_test(new UnitTest("bigint", "unsigned_char_parsing", unsigned_char_parsing));
	add_test(new UnitTest("bigint", "uint32_parsing", uint32_parsing));
	add_test(new UnitTest("bigint", "string_conversion", string_conversion));
	add_test(new UnitTest("bigint", "basic_addition", basic_addition));
	add_test(new UnitTest("bigint", "carry_addition", carry_addition));

	//Hashes
	//SHA256
	add_test(new UnitTest("sha256", "reset", sha256_reset));
	add_test(new UnitTest("sha256", "basic_hash", sha256_basic_hash));
	add_test(new UnitTest("sha256", "blocksize_hash", sha256_blocksize_hash));

	//SHA512
	add_test(new UnitTest("sha512", "reset", sha512_reset));
	add_test(new UnitTest("sha512", "basic_hash", sha512_basic_hash));
	add_test(new UnitTest("sha512", "blocksize_hash", sha512_blocksize_hash));
}

void UnitTests::run_all_tests() {
	uint64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	int successful_tests = 0;
	int failed_tests = 0;
	int total_tests = m_tests.size();
	for (UnitTest* test : m_tests) {
		TEST_RESULT test_result = test->run_test();
		if (test_result == TEST_RESULT::TEST_PASSED) successful_tests++;
		if (test_result == TEST_RESULT::TEST_FAILED) failed_tests++;
	}
	uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	uint64_t duration = end - start;
	std::cout << std::to_string(successful_tests) + "/" + std::to_string(total_tests) + " tests passed in " + std::to_string(duration) + "ms" << std::endl;
}

TEST_RESULT UnitTests::run_test(std::string name) {
	for (UnitTest* test : m_tests) {
		if (test->name() == name) return test->run_test();
	}
	return TEST_RESULT::TEST_DOES_NOT_EXIST;
}

void UnitTests::add_test(UnitTest* test) {
	m_tests.push_back(test);
}