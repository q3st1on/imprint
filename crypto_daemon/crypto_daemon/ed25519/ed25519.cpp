#include "ed25519.h"

#include "../hash/sha512.h"
#include "../bigint/bigint.h"

#include <iostream>
#include <chrono>

ed25519::Keypair ed25519::create_keypair(std::vector<unsigned char> seed) {
	int b = 256;

	if (seed.size() != 32) throw "Seed must be 32 bytes!";
	SHA512* sha512 = new SHA512;
	std::vector<unsigned char> private_key = sha512->hash(seed);
	
	//Derive public key
	//a = 2 ** (b - 2) + summation (calculated in the below while loop)
	BigInt b_minus_two(false, (uint32_t) b - 2);
	BigInt a = BigInt::TWO;
	a = a.pow(b_minus_two);

	//Summation
	BigInt base = BigInt::TWO;
	BigInt i(false, 3);
	BigInt max(false, (uint32_t) b - 3);

	int i_si = 3; //Always equals i but as a standard integer. This is so we don't have to do costly conversions from BigInt to int every iteration
	while (i <= max) {
		bool bit = 1 & (private_key[i_si / 8] >> (7 - (i_si % 8)));
		if (bit) {
			a = a + base.pow(i);
		}

		i = i + BigInt::ONE;
		i_si++;
	}

	//A = aB
	std::cout << B.x().value() << std::endl;
	std::cout << B.y().value() << std::endl;

	CurvePoint B_dbl = B.dbl();
	std::cout << B_dbl.x().value() << std::endl;
	std::cout << B_dbl.y().value() << std::endl;

	Keypair keypair = Keypair(private_key, private_key);
	return keypair;
}