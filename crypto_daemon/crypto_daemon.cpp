#include <iostream>

#include "hash/sha256.h"

int main(int argc, char** argv) {
	SHA256 sha256;
	std::vector<unsigned char> message = {'t', 'e', 's', 't'};
	std::vector<unsigned char> digest = sha256.hash(message);
	for (int i = 0; i < 32; i++) printf("%02x", digest[i]);

	//std::cout << (BigInt("1103950607238") / BigInt("8942")).value() << std::endl;
	//std::vector<unsigned char> seed = { 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34 };
	//ed25519::Keypair keypair = ed25519::create_keypair(seed);
}
