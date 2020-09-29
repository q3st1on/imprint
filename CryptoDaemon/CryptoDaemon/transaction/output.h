#pragma once
class Output {
	unsigned char m_destination[32];
	int m_amount;

public:
	unsigned char* destination();
	int amount();
};