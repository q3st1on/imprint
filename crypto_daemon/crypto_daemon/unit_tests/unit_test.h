#pragma once
#include <string>

enum class TEST_RESULT {
	TEST_PASSED,
	TEST_FAILED,
	TEST_DOES_NOT_EXIST
};

typedef TEST_RESULT(*TEST_POINTER)();

class UnitTest {
	std::string m_group;
	std::string m_name;
	TEST_POINTER m_function;

public:
	UnitTest(std::string, std::string, TEST_POINTER);

	TEST_RESULT run_test();

	//Helper functions
	std::string name();
	void set_name(std::string);
	std::string group();
	void set_group(std::string);
	TEST_POINTER function();
	void set_function(TEST_POINTER);
};