#include <iostream>
namespace glibby
{
	struct ComplexNumber
	{
		float a;
		float b;
	};
	
	inline ComplexNumber operator+(const ComplexNumber& num1, const ComplexNumber& num2)
	{
		float sum_a = num1.a + num2.a;
		float sum_b = num1.b + num2.b;
		return { sum_a, sum_b };
	}
	inline ComplexNumber operator-(const ComplexNumber& num1, const ComplexNumber& num2)
	{
		float diff_a = num1.a - num2.a;
		float diff_b = num1.b - num2.b;
		return { diff_a, diff_b };
	}
	inline ComplexNumber operator*(const ComplexNumber& num1, const ComplexNumber& num2)
	{
		float prod_a = (num1.a * num2.a) - (num1.b * num2.b);
		float prod_b = (num1.a * num2.b) + (num1.b * num2.a);
		return { prod_a, prod_b };
	}
	inline ComplexNumber operator/(const ComplexNumber& num1, const ComplexNumber& num2)
	{
		float quot_a = (num1.a * num2.a + num1.b * num2.b) / (num2.a * num2.a + num2.b * num2.b);
		float quot_b = (num1.b * num2.a - num1.a * num2.b) / (num2.a * num2.a + num2.b * num2.b);
		return { quot_a, quot_b };
	}
	inline bool operator==(const ComplexNumber& num1, const ComplexNumber& num2)
	{
		return num1.a == num2.a &&
			num1.b == num2.b;
	}
	inline bool operator!=(const ComplexNumber& num1, const ComplexNumber& num2)
	{
		return !(num1 == num2);
	}

	ComplexNumber conjugate(const ComplexNumber& num);
	ComplexNumber complex_pow(const ComplexNumber& base, const ComplexNumber& power);
	float arg(const ComplexNumber& num);
	float absolute(const ComplexNumber& num);
	std::ostream& operator<<(std::ostream& os, const ComplexNumber& num);
}