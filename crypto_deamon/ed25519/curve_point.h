#pragma once
#include "../bigint/bigint.h"

class CurvePoint {
	BigInt m_x;
	BigInt m_y;

public:
	CurvePoint(BigInt, BigInt);
	CurvePoint() = default;

	//Operations
	CurvePoint add(CurvePoint);
	CurvePoint dbl();
	CurvePoint multiply(BigInt);

	BigInt x();
	void set_x(BigInt);
	BigInt y();
	void set_y(BigInt);

};