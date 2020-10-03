#include <iostream>
#include <vector>

#include "ed25519/ed25519.h"
#include "bigint/bigint.h"

#include "unit_tests/unit_tests.h"

int main(int argc, char** argv) {
	BigInt* bigint1 = new BigInt("14");
	BigInt* bigint2 = new BigInt("7");
	//bigint1->subtract(bigint2);
	//std::cout << bigint1->value() << std::endl;

	UnitTests::load_tests();
	UnitTests::run_all_tests();
}