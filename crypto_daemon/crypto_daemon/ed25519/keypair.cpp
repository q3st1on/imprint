#include "keypair.h"

ed25519::Keypair::Keypair(std::vector<unsigned char> public_key, std::vector<unsigned char> private_key) {
	this->set_public_key(public_key);
	this->set_private_key(private_key);
}

std::vector<unsigned char> ed25519::Keypair::public_key() {
	return this->m_public_key;
}

std::vector<unsigned char> ed25519::Keypair::private_key() {
	return this->m_private_key;
}

void ed25519::Keypair::set_public_key(std::vector<unsigned char> public_key) {
	if (public_key.size() != 32) throw "Public key must be 32 bytes!";
	this->m_public_key.swap(public_key);
}

void ed25519::Keypair::set_private_key(std::vector<unsigned char> private_key) {
	if (private_key.size() != 64) throw "Private key must be 64 bytes!";
	this->m_private_key.swap(private_key);
}