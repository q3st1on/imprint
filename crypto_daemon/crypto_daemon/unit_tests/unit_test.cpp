#include "unit_test.h"

#include <iostream>

UnitTest::UnitTest(std::string group, std::string name, TEST_POINTER function) {
	this->set_group(group);
	this->set_name(name);
	this->set_function(function);
}

TEST_RESULT UnitTest::run_test() {
	TEST_RESULT test_result = (*this->function())();
	if (test_result == TEST_RESULT::TEST_PASSED) {
		std::cout << group() + ":" + name() << " passed!" << std::endl;
	} else {
		std::cout << group() + ":" + name() << " failed!" << std::endl;
	}

	return test_result;
}

std::string UnitTest::name() {
	return this->m_name;
}

void UnitTest::set_name(std::string name) {
	this->m_name = name;
}

std::string UnitTest::group() {
	return this->m_group;
}

void UnitTest::set_group(std::string group) {
	this->m_group = group;
}

TEST_POINTER UnitTest::function() {
	return this->m_function;
}

void UnitTest::set_function(TEST_POINTER function) {
	this->m_function = function;
}