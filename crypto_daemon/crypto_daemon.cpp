#include <iostream>

#include "bigint/bigint.h"
#include "ed25519/ed25519.h"

int main(int argc, char** argv) {
	std::string N;
	std::string D;
	std::cout << "enter N: \n";
	std::getline(std::cin, N);
	std::cout << "enter D: \n";
	std::getline(std::cin, D);
	std::cout << (BigInt(N) / BigInt(D)).value() << std::endl;



/*	std::vector<unsigned char> seed = { 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34 };
	ed25519::Keypair keypair = ed25519::create_keypair(seed); */
}
