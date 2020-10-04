#include "bigint.h"
#include "../hash/sha256.h"

#include <iostream>

const BigInt BigInt::ZERO = BigInt("0");
const BigInt BigInt::ONE = BigInt("1");
const BigInt BigInt::TWO = BigInt("2");

std::vector<uint32_t> BigInt::get_lower_digits(BigInt number) {
	if (this->digits().size() < number.digits().size()) return this->digits();
	return number.digits();
}

std::vector<uint32_t> BigInt::get_higher_digits(BigInt number) {
	if (this->digits().size() > number.digits().size()) return this->digits();
	return number.digits();
}

size_t BigInt::get_higher_digit_count(BigInt number) {
	return this->get_higher_digits(number).size();
}

BigInt::BigInt(std::string value) {
	bool negative = false;
	if (value[0] == '-') {
		negative = true;
		value.erase(0, 1);
	}

	int length = value.length();
	int lowest_multiple_of_nine = ((length / 9) + 1) * 9;
	if ((length % 9) == 0) lowest_multiple_of_nine = length; //If number was already a multiple of 9, then it is the lowest multiple
	
	//Left pad string with zeros resulting in a string with a number of digits that is a multiple of 9 (each uint32_t represents 9 digits)
	int left_padding_amount = lowest_multiple_of_nine - length;
	value = std::string(left_padding_amount, '0') + value;

	//Generate digits from padded string
	int chunk_count = value.length() / 9;
	std::vector<uint32_t> digits;
	for (int i = 0; i < chunk_count; i++) {
		std::string chunk = value.substr(i * 9, 9);
		uint32_t digit = std::stoi(chunk);
		digits.push_back(digit);
	}

	this->set_negative(negative);
	this->set_digits(digits);
}

BigInt::BigInt(bool negative, std::vector<uint32_t> digits) {
	this->set_negative(negative);
	this->set_digits(digits);
}

BigInt::BigInt(bool negative, std::vector<unsigned char> original_digits) {
	BigInt total = this->ZERO;
	BigInt current_bit_value = this->ONE;
	BigInt bit("0");

	int digit_count = original_digits.size();
	for (int i = (digit_count - 1); i >= 0; i--) {
		unsigned char digit = original_digits[i];
		for (int j = 0; j < 8; j++) {
			if (1 & (digit >> j)) {
				//Bit is set, add 2 ** bit_number to total
				total = total + current_bit_value;
			}
			current_bit_value = current_bit_value * this->TWO;
			bit = bit + this->ONE;
		}
	}

	this->set_negative(negative);
	this->set_digits(total.digits());
}

BigInt BigInt::operator+(BigInt number) {
	if (this->negative() && number.negative()) {
		//-x + -y = -x - y = -(y - -x) = -(y + x)
		return -(-number + -*this);
	}

	if (this->negative() && !number.negative()) {
		//-x + y = y - x
		return number - -*this;
	}

	if (!this->negative() && number.negative()) {
		//x + -y = x - y
		return *this - -number;
	}

	int higher_digit_count = this->get_higher_digit_count(number);
	std::vector<uint32_t> new_digits;
	uint64_t carry = 0;
	for (int i = 0; i < higher_digit_count; i++) {
		uint64_t new_digit = (uint64_t) this->get_digit_from_back(i) + (uint64_t) number.get_digit_from_back(i) + carry;
		carry = 0;
		if (new_digit > this->MAX_DIGIT_AMOUNT) {
			new_digit %= ((uint64_t) this->MAX_DIGIT_AMOUNT) + 1;
			carry = 1;
		}
		new_digits.insert(new_digits.begin(), (uint32_t) new_digit);
	}
	if (carry != 0) new_digits.insert(new_digits.begin(), (uint32_t) carry);
	return BigInt(false, new_digits);
}

BigInt BigInt::operator-(BigInt number) {
	if (this->negative() && number.negative()) {
		//-x - -y = -x + y = y - x
		return -number - -*this;
	}

	if (this->negative() && !number.negative()) {
		//-x - y = -(x + y)
		return -(-*this + number);
	}

	if (!this->negative() && number.negative()) {
		//x - -y = x + -y
		return *this + -number;
	}

	if (*this < number) {
		//Result would be negative, calculate result by flipping sides of operation and negating result -(number - this)
		return -(number - *this);
	}

	BigInt complement = number.get_complement();
	BigInt result = *this + complement;
	std::string value = result.value();
	value.erase(0, 1); //Remove extra 1 from beginning of answer
	return BigInt(value);
}

BigInt BigInt::operator*(BigInt number) {
	if (this->negative() && number.negative()) {
		//-x * -y = x * y
		return -number * -*this;
	}

	if (this->negative() && !number.negative()) {
		//-x * y = -(x * y)
		return -(-*this * number);
	}

	if (!this->negative() && number.negative()) {
		//x * -y = -(x * y)
		return -(*this * -number);
	}
	
	BigInt result("0");
	while (number != this->ZERO) {
		result = result + *this;
		number = number - this->ONE;
	}
	return result;
}

BigInt BigInt::operator/(BigInt number) {
	if (this->negative() && number.negative()) {
		//-x / -y = x / y
		return -*this / -number;
	}

	if (this->negative() && !number.negative()) {
		//-x / y = -(x / y)
		return -(-*this / number);
	}

	if (!this->negative() && number.negative()) {
		//x / -y = -(x / y)
		return -(*this / -number);
	}

	BigInt result("0");
	BigInt left_side = *this;
	while (left_side >= number) {
		result = result + this->ONE;
		left_side = left_side - number;
	}

	return result;
}

BigInt BigInt::pow(BigInt exponent) {
	if (this->negative()) {
		BigInt positive_base = -(*this);
		return -(positive_base.pow(exponent));
	}

	BigInt result = this->ONE;
	while (exponent != this->ZERO) {
		result = result * *this;
		exponent = exponent - this->ONE;
	}
	return result;
}

BigInt BigInt::operator-() {
	BigInt negated_bigint = BigInt(!this->negative(), this->digits());
	return negated_bigint;
}

bool BigInt::operator==(BigInt number) {
	if (this->digits().size() != number.digits().size()) return false; //Numbers have different amount of digits so they can't be equal

	//Compare each digit manually
	int digit_count = this->digits().size();
	for (int i = 0; i < digit_count; i++) {
		uint32_t my_digit = this->digits()[i];
		uint32_t other_digit = number.digits()[i];
		if (my_digit != other_digit) return false;
	}

	return true;
}

bool BigInt::operator!=(BigInt number) {
	return !(*this == number);
}

bool BigInt::operator>(BigInt number) {
	if (this->negative() && !number.negative()) return false; //If we are negative and the other number is positive, we cannot be bigger than the other number
	if (!this->negative() && number.negative()) return false; //If we are positive and the other number is negative, we must be bigger than the other number
	
	if (this->negative() && number.negative()) {
		return (-*this) < (-number); //If both numbers are negative, we are bigger if we are smaller when both signs are removed
	}

	//Number is positive
	if (this->digits().size() > number.digits().size()) return true; //We have more digits and therefore must be bigger
	if (this->digits().size() < number.digits().size()) return true; //We have less digits and therefore must be smaller

	//Both numbers have same amount of digits, compare each individually
	int digit_count = this->digits().size();
	for (int i = 0; i < digit_count; i++) {
		uint32_t my_digit = this->digits()[i];
		uint32_t other_digit = number.digits()[i];

		if (my_digit > other_digit) return true;
		if (my_digit < other_digit) return false;
	}

	return false; //Values are the same, therefore this number is not bigger
}

bool BigInt::operator<(BigInt number) {
	return !(*this > number) && !(*this == number); //If we are not bigger than the other number, and not equal to the other number, we must be less than the other number
}

bool BigInt::operator>=(BigInt number) {
	return (*this > number) || (*this == number);
}

bool BigInt::operator<=(BigInt number) {
	return (*this < number) || (*this == number);
}

std::vector<uint32_t> BigInt::digits() {
	return this->m_digits;
}

void BigInt::set_digits(std::vector<uint32_t> digits) {
	this->m_digits.swap(digits);
}

std::string BigInt::value() {
	std::string value = this->negative() ? "-" : "";
	int digit_count = this->digits().size();
	for (int i = 0; i < digit_count; i++) {
		std::string final_value = std::to_string(this->digits()[i]);
		if (i != 0) final_value = std::string(9 - final_value.length(), '0') + final_value;
		value += final_value;
	}
	return value;
}

uint32_t BigInt::get_digit_from_back(std::vector<uint32_t> digits, int digits_from_back) {
	//Get digit n digits from last digit (returns 0 if digit doesn't exist)
	int index = digits.size() - digits_from_back - 1;
	if (index < 0) return 0;
	return digits[index];
}

uint32_t BigInt::get_digit_from_back(int digits_from_back) {
	return this->get_digit_from_back(this->digits(), digits_from_back);
}

BigInt BigInt::get_complement() {
	std::vector<uint32_t> complement_digits;
	int digit_count = this->digits().size();
	for (int i = 0; i < digit_count; i++) {
		uint32_t digit = this->digits()[i];
		complement_digits.push_back(this->MAX_DIGIT_AMOUNT - digit);
	}
	BigInt complement(false, complement_digits);
	return complement + this->ONE;
}

bool BigInt::negative() {
	return this->m_negative;
}

void BigInt::set_negative(bool negative) {
	this->m_negative = negative;
}
