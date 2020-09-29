#pragma once
#include <cstdint>

class Input {
	uint16_t m_height;
	unsigned char m_hash[32];
	uint8_t m_index;

public:
	uint16_t height();
	unsigned char* hash();
	uint8_t index();
};