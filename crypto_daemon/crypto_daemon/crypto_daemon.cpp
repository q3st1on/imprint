#include <iostream>
#include <vector>

#include "ed25519/ed25519.h"
#include "bigint/bigint.h"

#include "unit_tests/unit_tests.h"

int main(int argc, char** argv) {
	UnitTests::load_tests();
	UnitTests::run_all_tests();
}