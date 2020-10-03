#pragma once
#include <vector>

namespace ed25519 {
	class Keypair {
		std::vector<unsigned char> m_public_key;
		std::vector<unsigned char> m_private_key;
	
	public:
		Keypair(std::vector<unsigned char>, std::vector<unsigned char>);

		std::vector<unsigned char> public_key();
		std::vector<unsigned char> private_key();

		void set_public_key(std::vector<unsigned char> public_key);
		void set_private_key(std::vector<unsigned char> private_key);
	};
}