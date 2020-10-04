#pragma once
#include <vector>
#include <string>

class BigInt {

	bool m_negative = false;
	std::vector<uint32_t> m_digits;

	uint32_t MAX_DIGIT_AMOUNT = 999999999; //This is 10^9 - 1 because we are operating in base 10^9 and zero is included as a digit

	std::vector<uint32_t> get_lower_digits(BigInt);  //Returns BigInt with less digits
	std::vector<uint32_t> get_higher_digits(BigInt); //Returns BigInt with more digits
	size_t get_higher_digit_count(BigInt);           //Returns number of digits BigInt with most digits has

public:
	//Static properties
	static const BigInt ZERO;
	static const BigInt ONE;
	static const BigInt TWO;

	//Constructors
	BigInt(std::string);                      //Construct via string                                     "-27"  = -27
	BigInt(bool, std::vector<uint32_t>);      //Construct via 32 bit unsigned integer vector (base 10^9) {1, 5} = 10^9 + 5
	BigInt(bool, std::vector<unsigned char>); //Construct via 8 bit unsigned character vector            {1, 5} = (1 << 8) + 5

	//Operations
	BigInt operator+(BigInt); //Add
	BigInt operator-(BigInt); //Subtract
	BigInt operator*(BigInt); //Multiply
	BigInt operator/(BigInt); //Divide

	BigInt pow(BigInt);

	BigInt operator-();       //Negate

	bool operator==(BigInt);   //Equal to
	bool operator!=(BigInt);   //Not equal to
	bool operator>(BigInt);   //Greater than
	bool operator<(BigInt);   //Less than
	bool operator>=(BigInt);   //Greater than or equal to
	bool operator<=(BigInt);   //Less than or equal to

	//Helper functions
	std::vector<uint32_t> digits();
	void set_digits(std::vector<uint32_t>);
	std::string value();
	static uint32_t get_digit_from_back(std::vector<uint32_t>, int);
	uint32_t get_digit_from_back(int);
	BigInt get_complement();

	bool negative();
	void set_negative(bool);
};