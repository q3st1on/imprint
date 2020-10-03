#pragma once
#include <vector>

#include "keypair.h"

namespace ed25519 {

	Keypair create_keypair(std::vector<unsigned char>);
}