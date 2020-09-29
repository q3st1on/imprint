#pragma once
#include <vector>

#include "input.h"
#include "output.h"

class Transaction {
	std::vector<Input> m_inputs;
	std::vector<Output> m_outputs;

public:
	std::vector<Input> inputs();
	std::vector<Output> outputs();

	size_t get_input_count();
	size_t get_output_count();
};

