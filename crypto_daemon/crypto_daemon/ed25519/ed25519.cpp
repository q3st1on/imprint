#include "ed25519.h"

#include "../hash/sha512.h"

ed25519::Keypair ed25519::create_keypair(std::vector<unsigned char> seed) {
	if (seed.size() != 32) throw "Seed must be 32 bytes!";
	SHA512* sha512 = new SHA512;
	std::vector<unsigned char> digest = sha512->hash(seed);
	Keypair keypair = Keypair(digest, digest);
	return keypair;
}