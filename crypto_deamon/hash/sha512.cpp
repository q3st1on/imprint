#include "sha512.h"

#include <cstring>
#include <bit>

uint64_t SHA512::pack(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7, uint8_t byte8) {
	return ((uint64_t) byte1) << 0 |
		((uint64_t) byte2) << 8 |
		((uint64_t) byte3) << 16 |
		((uint64_t) byte4) << 24 |
		((uint64_t) byte5) << 32 |
		((uint64_t) byte6) << 40 |
		((uint64_t) byte7) << 48 |
		((uint64_t) byte8) << 56;
}

void SHA512::init() {
	this->h0 = 0x6a09e667f3bcc908;
	this->h1 = 0xbb67ae8584caa73b;
	this->h2 = 0x3c6ef372fe94f82b;
	this->h3 = 0xa54ff53a5f1d36f1;
	this->h4 = 0x510e527fade682d1;
	this->h5 = 0x9b05688c2b3e6c1f;
	this->h6 = 0x1f83d9abfb41bd6b;
	this->h7 = 0x5be0cd19137e2179;
	this->message.clear();
}

void SHA512::update(std::vector<unsigned char> message) {
	this->message.insert(this->message.end(), message.begin(), message.end());
}

std::vector<unsigned char> SHA512::final() {
	uint64_t message_size = this->message.size();
	this->message.push_back(1 << 7);

	int desired_size = (((this->message.size() / 128) + 1) * 128) - 16;
	int new_element_count = desired_size - (int)message_size - 1 + 8; //Add 8 because we don't have a 128 bit integer type, message length shouldn't be over 64 bits but if it is this will fail

	for (int i = 0; i < new_element_count; i++) {
		this->message.push_back(0);
	}

	uint64_t size_in_bits = message_size * 8;
	for (int i = 7; i >= 0; i--) {
		unsigned char append_byte = (size_in_bits >> (8 * i)) & 0xff;;
		this->message.push_back(append_byte);
	}

	int chunk_count = this->message.size() / 128;
	for (int chunk = 0; chunk < chunk_count; chunk++) {
		uint64_t w[80];
		memset(w, 0, sizeof(uint64_t) * 80);

		for (int i = 0; i < 16; i++) {
			int base = (chunk * 128) + (i * 8);
			unsigned char* p = (unsigned char*) (&w[i]);
			w[i] = this->pack(this->message[base + 7], this->message[base + 6], this->message[base + 5], this->message[base + 4], this->message[base + 3], this->message[base + 2], this->message[base + 1], this->message[base]);
		}

		for (int i = 16; i < 80; i++) {
			uint64_t s0 = std::rotr(w[i - 15], 1) ^ std::rotr(w[i - 15], 8) ^ (w[i - 15] >> 7);
			uint64_t s1 = std::rotr(w[i - 2], 19) ^ std::rotr(w[i - 2], 61) ^ (w[i - 2] >> 6);
			w[i] = (w[i - 16] + s0 + w[i - 7] + s1);
		}

		uint64_t a = h0;
		uint64_t b = h1;
		uint64_t c = h2;
		uint64_t d = h3;
		uint64_t e = h4;
		uint64_t f = h5;
		uint64_t g = h6;
		uint64_t h = h7;

		for (int i = 0; i < 80; i++) {
			uint64_t S1 = std::rotr(e, 14) ^ std::rotr(e, 18) ^ std::rotr(e, 41);
			uint64_t ch = (e & f) ^ ((~e) & g);
			uint64_t temp1 = h + S1 + ch + k[i] + w[i];
			uint64_t S0 = std::rotr(a, 28) ^ std::rotr(a, 34) ^ std::rotr(a, 39);
			uint64_t maj = (a & b) ^ (a & c) ^ (b & c);
			uint64_t temp2 = S0 + maj;

			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}

		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;
		h4 += e;
		h5 += f;
		h6 += g;
		h7 += h;
	}

	std::vector<uint64_t> digest;
	digest.push_back(h0);
	digest.push_back(h1);
	digest.push_back(h2);
	digest.push_back(h3);
	digest.push_back(h4);
	digest.push_back(h5);
	digest.push_back(h6);
	digest.push_back(h7);

	std::vector<unsigned char> final_digest;
	for (int i = 0; i < 8; i++) {
		uint64_t packed_data = digest[i];
		for (int i = 0; i < 8; i++) {
			final_digest.push_back((packed_data >> 8 * (7 - i)) & 0xFF);
		}
	}

	return final_digest;
}

std::vector<unsigned char> SHA512::hash(std::vector<unsigned char> message) {
	this->init();
	this->update(message);
	return this->final();
}
