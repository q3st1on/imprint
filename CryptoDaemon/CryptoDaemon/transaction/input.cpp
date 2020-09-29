#include "input.h"

uint16_t Input::height() {
	return this->m_height;
}

unsigned char* Input::hash() {
	return this->m_hash;
}

uint8_t Input::index() {
	return this->m_index;
}