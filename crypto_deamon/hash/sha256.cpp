#include "sha256.h"

#include <cstring>
#include <bit>

uint32_t SHA256::pack(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) {
	return byte1 << 0 |
		byte2 << 8 |
		byte3 << 16 |
		byte4 << 24;
}

void SHA256::init() {
	this->h0 = 0x6a09e667;
	this->h1 = 0xbb67ae85;
	this->h2 = 0x3c6ef372;
	this->h3 = 0xa54ff53a;
	this->h4 = 0x510e527f;
	this->h5 = 0x9b05688c;
	this->h6 = 0x1f83d9ab;
	this->h7 = 0x5be0cd19;
	this->message.clear();
}

void SHA256::update(std::vector<unsigned char> message) {
	this->message.insert(this->message.end(), message.begin(), message.end());
}

std::vector<unsigned char> SHA256::final() {
	uint64_t message_size = this->message.size();
	this->message.push_back(1 << 7);

	int desired_size = (((this->message.size() / 64) + 1) * 64) - 8;
	int new_element_count = desired_size - (int) message_size - 1;

	for (int i = 0; i < new_element_count; i++) {
		this->message.push_back(0);
	}

	uint64_t size_in_bits = message_size * 8;
	for (int i = 7; i >= 0; i--) {
		unsigned char append_byte = (size_in_bits >> (8 * i)) & 0xff;;
		this->message.push_back(append_byte);
	}

	int chunk_count = this->message.size() / 64;
	for (int chunk = 0; chunk < chunk_count; chunk++) {
		uint32_t w[64];
		memset(w, 0, sizeof(uint32_t) * 64);

		for (int i = 0; i < 16; i++) {
			int base = (chunk * 64) + (i * 4);
			w[i] = this->pack(this->message[base + 3], this->message[base + 2], this->message[base + 1], this->message[base]);
		}

		for (int i = 16; i < 64; i++) {
			uint32_t s0 = std::rotr(w[i - 15], 7) ^ std::rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
			uint32_t s1 = std::rotr(w[i - 2], 17) ^ std::rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
			w[i] = (w[i - 16] + s0 + w[i - 7] + s1);
		}

		uint32_t a = h0;
		uint32_t b = h1;
		uint32_t c = h2;
		uint32_t d = h3;
		uint32_t e = h4;
		uint32_t f = h5;
		uint32_t g = h6;
		uint32_t h = h7;

		for (int i = 0; i < 64; i++) {
			uint32_t S1 = std::rotr(e, 6) ^ std::rotr(e, 11) ^ std::rotr(e, 25);
			uint32_t ch = (e & f) ^ ((~e) & g);
			uint32_t temp1 = h + S1 + ch + k[i] + w[i];
			uint32_t S0 = std::rotr(a, 2) ^ std::rotr(a, 13) ^ std::rotr(a, 22);
			uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
			uint32_t temp2 = S0 + maj;

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

	std::vector<uint32_t> digest;
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
		for (int i = 0; i < 4; i++) {
			final_digest.push_back((packed_data >> 8 * (3 - i)) & 0xFF);
		}
	}

	return final_digest;
}

std::vector<unsigned char> SHA256::hash(std::vector<unsigned char> message) {
	this->init();
	this->update(message);
	return this->final();
}
