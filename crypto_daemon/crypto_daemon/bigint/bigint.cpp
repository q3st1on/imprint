#include "bigint.h"
#include "../hash/sha256.h"

#include <iostream>

std::vector<uint32_t> BigInt::get_lower_digit_count(BigInt* number) {
	if (this->digits().size() < number->digits().size()) return this->digits();
	return number->digits();
}

std::vector<uint32_t> BigInt::get_higher_digit_count(BigInt* number) {
	if (this->digits().size() > number->digits().size()) return this->digits();
	return number->digits();
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
	std::vector<uint32_t> converted_digits;
	int padding_element_count = (((original_digits.size() / 4) + 1) * 4) - original_digits.size();
	if ((original_digits.size() % 4) == 0) padding_element_count -= 4;
	for (int i = 0; i < padding_element_count; i++) {
		original_digits.insert(original_digits.begin(), 0);
	}
	int packed_element_count = original_digits.size() / 4;
	for (int i = 0; i < packed_element_count; i++) {
		int base = i * 4;
		uint32_t digit = SHA256::pack(original_digits[base + 3], original_digits[base + 2], original_digits[base + 1], original_digits[base]);
		if (digit > this->MAX_DIGIT_AMOUNT) throw "Invalid digits!";
		converted_digits.push_back(digit);
	}
	this->set_digits(converted_digits);
}

void BigInt::add(BigInt* number) {
	std::vector<uint32_t> my_digits = this->get_higher_digit_count(number);
	std::vector<uint32_t> other_digits = this->get_lower_digit_count(number);
	
	int my_digit_count = my_digits.size();
	std::vector<uint32_t> new_digits;
	uint64_t carry = 0;
	for (int i = 0; i < my_digit_count; i++) {
		uint64_t new_digit = (uint64_t) this->get_digit_from_back(i) + (uint64_t) number->get_digit_from_back(i) + carry;
		carry = 0;
		if (new_digit > this->MAX_DIGIT_AMOUNT) {
			carry = new_digit - this->MAX_DIGIT_AMOUNT;
			new_digit = this->MAX_DIGIT_AMOUNT;
		}
		new_digits.insert(new_digits.begin(), (uint32_t) new_digit);
	}
	if (carry != 0) new_digits.insert(new_digits.begin(), (uint32_t) carry);
	this->set_digits(new_digits);
}

std::vector<uint32_t> BigInt::digits() {
	return this->m_digits;
}

void BigInt::set_digits(std::vector<uint32_t> digits) {
	this->m_digits.swap(digits);
}

std::string BigInt::value() {
	std::string value = "";
	int digit_count = this->digits().size();
	for (int i = 0; i < digit_count; i++) {
		std::string final_value = std::to_string(this->digits()[i]);
		if (i != 0) final_value = std::string(9 - final_value.length(), '0') + final_value;
		value += final_value;
	}
	return value;
}

uint32_t BigInt::get_digit_from_back(int digits_from_back) {
	//Get digit n digits from last digit (returns 0 if digit doesn't exist)
	int index = this->digits().size() - digits_from_back - 1;
	if (index < 0) return 0;
	return this->digits()[index];
}

bool BigInt::negative() {
	return this->m_negative;
}

void BigInt::set_negative(bool negative) {
	this->m_negative = negative;
}
