#include "curve_point.h"
#include "ed25519.h"

CurvePoint::CurvePoint(BigInt x, BigInt y) {
	this->set_x(x);
	this->set_y(y);
}

CurvePoint CurvePoint::add(CurvePoint curve_point) {

}
#include <iostream>
CurvePoint CurvePoint::dbl() {
	std::cout << "in dbl" << std::endl;
	std::cout << "here" << std::endl;
	BigInt x_squared = this->x().pow(BigInt::TWO);
	std::cout << "squared x" << std::endl;
	BigInt threex_squared = x_squared * BigInt::THREE;
	std::cout << "threex squared" << std::endl;
	BigInt s_tophalf = threex_squared + ed25519::a;
	std::cout << "s_tophalf" << std::endl;
	BigInt two = BigInt::TWO;
	BigInt s_bottomhalf = two * this->y();
	std::cout << "two times y" << std::endl;
	std::cout << s_tophalf.value() << std::endl;
	std::cout << s_bottomhalf.value() << std::endl;
	BigInt s = s_tophalf / s_bottomhalf;

	std::cout << "got s" << std::endl;
	BigInt x = s.pow(BigInt::TWO);
	BigInt two_origx = two * this->x();
	x = x - two_origx;
	std::cout << "got x" << std::endl;

	BigInt old_minus_new = this->x() - x;
	BigInt mult = s * old_minus_new;
	BigInt y = mult - this->y();
	std::cout << "got y" << std::endl;

	return CurvePoint(x, y);
}

CurvePoint CurvePoint::multiply(BigInt number) {

}

BigInt CurvePoint::x() {
	return this->m_x;
}

void CurvePoint::set_x(BigInt x) {
	this->m_x = x;
}

BigInt CurvePoint::y() {
	return this->m_y;
}

void CurvePoint::set_y(BigInt y) {
	this->m_y = y;
}