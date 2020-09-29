#include "transaction.h"

std::vector<Input> Transaction::inputs() {
	return this->m_inputs;
}

std::vector<Output> Transaction::outputs() {
	return this->m_outputs;
}

size_t Transaction::get_input_count() {
	return this->m_inputs.size();
}

size_t Transaction::get_output_count() {
	return this->m_outputs.size();
}