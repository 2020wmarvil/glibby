#include "glibby/math/complex_math.h"
#include <cmath>
#include <iostream>
namespace glibby
{
	
	ComplexNumber conjugate(const ComplexNumber& num)
	{
		float con_a = num.a;
		float con_b = -num.b;
		return { con_a, con_b };
	}
	ComplexNumber complex_pow(const ComplexNumber& base, const ComplexNumber& power)
	{
		float c = pow(base.a * base.a + base.b * base.b, power.a *0.5) * exp(-power.b * arg(base));
		float a = cos(power.a * arg(base) +	0.5 * power.b * log(base.a * base.a + base.b * base.b));
		float b = sin(power.a * arg(base) + 0.5 * power.b * log(base.a * base.a + base.b * base.b));
		ComplexNumber result;
		result.a = c * a;
		result.b = c * b;
		return result;
	}
	float arg(const ComplexNumber& num)
	{
		return std::atan(num.b / num.a);
	}
	float absolute(const ComplexNumber& num)
	{
		return std::sqrt(num.a * num.a + num.b * num.b);
	}

	std::ostream& operator<<(std::ostream& os, const ComplexNumber& num)
	{
		os << num.a << " " << num.b << " ";
		return os;
	}

	std::istream& operator>>(std::istream& is, ComplexNumber& num)
	{
		is >> num.a;
		is >> num.b;
		return is;
	}
}