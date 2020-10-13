#include "bigint.h"
#include "../hash/sha256.h"

#include <iostream>

const BigInt BigInt::ZERO = BigInt("0");
const BigInt BigInt::ONE = BigInt("1");
const BigInt BigInt::TWO = BigInt("2");
const BigInt BigInt::THREE = BigInt("3");
const BigInt BigInt::HUNDRED_THOUSAND = BigInt("100000");
const BigInt BigInt::UINT32_LIMIT = BigInt("4294967295");

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

BigInt::BigInt(bool negative, uint32_t digit) {
	std::vector<uint32_t> digits = { digit };
	this->set_negative(false);
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

	BigInt complement = number.get_complement(this->digits().size());
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

	//Basic optimisation x * 1 = x
	if (*this == this->ONE) return number;
	if (number == this->ONE) return *this;

	//Basic optimisation x * 0 = 0
	if (*this == this->ZERO) return this->ZERO;
	if (number == this->ZERO) return this->ZERO;

	//Recursive exit condition (both numbers are less than the unsigned 32 bit integer limit, this is the chosen exit condition because we can compute these numbers natively)
	if ((*this < this->UINT32_LIMIT) && (number <= this->UINT32_LIMIT)) {
		uint64_t product = std::stoull(this->value()) * std::stoull(number.value());
		return BigInt(std::to_string(product));
	}

	//Karatsuba multiplication algorithm
	std::vector<uint32_t> this_digits = this->digits();
	std::vector<uint32_t> other_digits = number.digits();

	int max = std::max(this_digits.size(), other_digits.size());
	if ((max % 2) == 1) max += 1;
	int this_padding = max - this_digits.size();
	int other_padding = max - other_digits.size();

 	for (int i = 0; i < this_padding; i++) this_digits.insert(this_digits.begin(), 0);
	for (int i = 0; i < other_padding; i++) other_digits.insert(other_digits.begin(), 0);

	int middle = max / 2;
	std::vector<uint32_t> high_x(this_digits.begin(), this_digits.begin() + middle);
	std::vector<uint32_t> low_x(this_digits.begin() + middle, this_digits.begin() + this_digits.size());
	std::vector<uint32_t> high_y(other_digits.begin(), other_digits.begin() + middle);
	std::vector<uint32_t> low_y(other_digits.begin() + middle, other_digits.begin() + other_digits.size());
	BigInt x1 = BigInt(false, high_x);
	BigInt x0 = BigInt(false, low_x);
	BigInt y1 = BigInt(false, high_y);
	BigInt y0 = BigInt(false, low_y);

	BigInt z0 = x0 * y0;
	BigInt z2 = x1 * y1;
	BigInt z1 = (x1 + x0) * (y1 + y0) - z2 - z0;

	BigInt two_m(false, (uint32_t) max);
	BigInt m(false, (uint32_t) middle);

	BigInt one = z2 << two_m;
	BigInt two = z1 << m;
	BigInt result = one + two + z0;

	return result.remove_leading_zeros();
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

	std::cout << this->value() + " / " + number.value() << std::endl;

	if ((*this < this->UINT32_LIMIT) && (number <= this->UINT32_LIMIT)) {
		uint64_t result = std::stoull(this->value()) / std::stoull(number.value());
		return BigInt(std::to_string(result));
	}

	return this->THREE;
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

BigInt BigInt::operator<<(BigInt shift_amount) {
	std::vector<uint32_t> shifted_digits = this->digits();
	while (shift_amount > this->ZERO) {
		shifted_digits.push_back((uint32_t) 0);
		shift_amount = shift_amount - this->ONE;
	}
	return BigInt(false, shifted_digits);
}

bool BigInt::operator==(BigInt number) {
	BigInt stripped_this = this->remove_leading_zeros();
	BigInt stripped_other = number.remove_leading_zeros();
	if (stripped_this.digits().size() != stripped_other.digits().size()) return false; //Numbers have different amount of digits so they can't be equal

	//Compare each digit manually
	int digit_count = stripped_this.digits().size();
	for (int i = 0; i < digit_count; i++) {
		uint32_t my_digit = stripped_this.digits()[i];
		uint32_t other_digit = stripped_other.digits()[i];
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
	if (this->digits().size() < number.digits().size()) return false; //We have less digits and therefore must be smaller

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
	if (!this->negative() && number.negative()) return false; //If we are negative and the other number is positive, we cannot be smaller than the other number
	if (this->negative() && !number.negative()) return false; //If we are positive and the other number is negative, we must be smaller than the other number

	if (this->negative() && number.negative()) {
		return (-*this) > (-number); //If both numbers are negative, we are smaller if we are bigger when both signs are removed
	}

	//Number is positive
	if (this->digits().size() < number.digits().size()) return true; //We have less digits and therefore must be smaller
	if (this->digits().size() > number.digits().size()) return false; //We have more digits and therefore must be bigger

	//Both numbers have same amount of digits, compare each individually
	int digit_count = this->digits().size();
	for (int i = 0; i < digit_count; i++) {
		uint32_t my_digit = this->digits()[i];
		uint32_t other_digit = number.digits()[i];

		if (my_digit < other_digit) return true;
		if (my_digit > other_digit) return false;
	}

	return false; //Values are the same, therefore this number is not smaller
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

BigInt BigInt::get_complement(int length) {
	std::vector<uint32_t> complement_digits;
	int digit_count = this->digits().size();
	
	int padding_amount = length - digit_count;
	if (padding_amount < 0) padding_amount = 0;
	for (int i = 0; i < padding_amount; i++) complement_digits.push_back(this->MAX_DIGIT_AMOUNT);

	for (int i = 0; i < digit_count; i++) {
		uint32_t digit = this->digits()[i];
		complement_digits.push_back(this->MAX_DIGIT_AMOUNT - digit);
	}
	BigInt complement(false, complement_digits);
	return complement + this->ONE;
}

void BigInt::push_digit(uint32_t digit) {
	this->m_digits.push_back(digit);
}

BigInt BigInt::remove_leading_zeros() {
	bool leading = true;
	std::vector<uint32_t> stripped;
	int digit_count = this->digits().size();
	for (int i = 0; i < digit_count; i++) {
		uint32_t digit = this->digits()[i];
		if (leading) {
			if (digit != 0) {
				leading = false;
				stripped.push_back(digit);
			}
		} else {
			stripped.push_back(digit);
		}
	}
	
	return BigInt(this->negative(), stripped);
}

bool BigInt::negative() {
	return this->m_negative;
}

void BigInt::set_negative(bool negative) {
	this->m_negative = negative;
}
