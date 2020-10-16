#pragma once
#include <vector>

#include "keypair.h"
#include "curve_point.h"

namespace ed25519 {
	static CurvePoint B(BigInt("15112221349535400772501151409588531511454012693041857206046113283949847762202"), BigInt("46316835694926478169428394003475163141307993866256225615783033603165251855960"));
	static BigInt a("486662");

	Keypair create_keypair(std::vector<unsigned char>);
}