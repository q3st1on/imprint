#pragma once
#include <string>
#include <vector>

#include "unit_test.h"

class UnitTests {
	static std::vector<UnitTest*> m_tests;

public:
	static void load_tests();
	static void run_all_tests();
	static TEST_RESULT run_test(std::string);
	static void add_test(UnitTest*);
};