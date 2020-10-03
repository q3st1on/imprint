#pragma once
#include <vector>
#include <string>

class BigInt {

	bool m_negative = false;
	std::vector<uint32_t> m_digits;

	uint32_t MAX_DIGIT_AMOUNT = 999999999; //This is 10^9 - 1 because we are operating in base 10^9 and zero is included as a digit

	std::vector<uint32_t> get_lower_digit_count(BigInt*);
	std::vector<uint32_t> get_higher_digit_count(BigInt*);
public:
	//Constructors
	BigInt(std::string);                      //Construct via string                                     "-27"  = -27
	BigInt(bool, std::vector<uint32_t>);      //Construct via 32 bit unsigned integer vector (base 10^9) {1, 5} = 10^9 + 5
	BigInt(bool, std::vector<unsigned char>); //Construct via 8 bit unsigned character vector            {1, 5} = (1 << 8) + 5

	//Operations
	void add(BigInt*);
	
	//Helper functions
	std::vector<uint32_t> digits();
	void set_digits(std::vector<uint32_t>);
	std::string value();
	uint32_t get_digit_from_back(int);

	bool negative();
	void set_negative(bool);
};