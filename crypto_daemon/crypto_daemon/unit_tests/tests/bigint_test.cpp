#include "bigint_test.h"

#include "../../bigint/bigint.h"

//TODO: Add tests for multplication and division, also make tests use many more numbers

TEST_RESULT string_parsing() {
	BigInt bigint1("1000000001");
	if (bigint1.digits().size() == 2 && bigint1.digits()[0] == 1 && bigint1.digits()[1] == 1) return TEST_RESULT::TEST_PASSED;
	return TEST_RESULT::TEST_FAILED;
}

TEST_RESULT unsigned_char_parsing() {
	std::vector<unsigned char> digits = {1, 0, 0, 1, 0};
	BigInt bigint1(false, digits);
	if (bigint1.digits().size() == 2 && bigint1.digits()[0] == 1 && bigint1.digits()[1] == 256) return TEST_RESULT::TEST_PASSED;
	return TEST_RESULT::TEST_FAILED;
}

TEST_RESULT uint32_parsing() {
	std::vector<uint32_t> digits = {12, 15};
	BigInt bigint1(false, digits);
	if (bigint1.digits().size() == 2 && bigint1.digits()[0] == 12 && bigint1.digits()[1] == 15) return TEST_RESULT::TEST_PASSED;
	return TEST_RESULT::TEST_FAILED;
}

TEST_RESULT string_conversion() {
	std::vector<uint32_t> digits = { 1, 1 };
	BigInt bigint1(false, digits);
	if (bigint1.value() == "1000000001") return TEST_RESULT::TEST_PASSED;
	return TEST_RESULT::TEST_FAILED;
}

TEST_RESULT basic_addition() {
	std::vector<uint32_t> digits = { 1 };
	BigInt bigint1(false, digits);

	std::vector<uint32_t> digits2 = { 1 };
	BigInt bigint2(false, digits2);

	BigInt result = bigint1 + bigint2;

	if (result.digits()[0] == 2) return TEST_RESULT::TEST_PASSED;
	return TEST_RESULT::TEST_FAILED;
}

TEST_RESULT carry_addition() {
	std::vector<uint32_t> digits = { 9 };
	BigInt bigint1(false, digits);

	std::vector<uint32_t> digits2 = { 6 };
	BigInt bigint2(false, digits2);

	BigInt result = bigint1 + bigint2;

	if (result.digits()[0] == 15) return TEST_RESULT::TEST_PASSED;
	return TEST_RESULT::TEST_FAILED;
}