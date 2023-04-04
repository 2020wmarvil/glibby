//#include "glibby/math/polynomial.h"
#include "/Users/nikul/Desktop/copy_glibby/copy_glibby/include/glibby/math/polynomial.h"
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;
using namespace glibby;

int main(){
    
    vector<int> values1;
    values1.push_back(3);
    values1.push_back(4);
    values1.push_back(9);
    values1.push_back(1);

    vector<int> values2;
    values2.push_back(5);
    values2.push_back(1);
    values2.push_back(2); 

    vector<int> values3;
    values3.push_back(-1);
    values3.push_back(2);
    values3.push_back(-6);
    values3.push_back(2);

    vector<int> values4;
    values4.push_back(1);
    values4.push_back(3);
    values4.push_back(2);

    vector<int> values5;
    values5.push_back(2);
    values5.push_back(9);
    values5.push_back(4);

    vector<int> values6;
    values6.push_back(6);
    values6.push_back(5);
    values6.push_back(1);

    vector<int> values7;
    values7.push_back(-3);
    values7.push_back(-5);
    values7.push_back(2);
    
    Quadratic one(values1);   //  x^3 + 9x^2 + 4x + 3
    Quadratic two(values2);   // 2x^2 + x + 5
    Quadratic three(values3); // 2x^3 - 6x^2 + 2x - 1
    Quadratic four(values4);  // 2x^2 + 3x + 1
    Quadratic five(values5);  // 4x^2 + 9x^2 + 2
    Quadratic six(values6);   // x^2 + 5x + 6
    Quadratic seven(values7); // -3x^2 -5x + 2

    Quadratic sub_result = one.subtract(two);
    assert(sub_result.get_values()[0] == -2);
    assert(sub_result.get_values()[1] == 3);
    assert(sub_result.get_values()[2] == 7);
    assert(sub_result.get_values()[3] == 1);

    Quadratic add_result = one.add(two);
    assert(add_result.get_values()[0] == 8);
    assert(add_result.get_values()[1] == 5);
    assert(add_result.get_values()[2] == 11);
    assert(add_result.get_values()[3] == 1);

    Quadratic product = one.multiply(two);
    assert(product.get_values()[0] == 15);
    assert(product.get_values()[1] == 23);
    assert(product.get_values()[2] == 55);
    assert(product.get_values()[3] == 22);
    assert(product.get_values()[4] == 19);
    assert(product.get_values()[5] == 2);

    // evaluating value at given point check
    assert(one.evaluate(0) == 3);
    assert(two.evaluate(4) == 41);
    assert(three.evaluate(3) == 5);
    assert(four.evaluate(2) == 15);
    assert(five.evaluate(-9) == 245); 

    // evaluating horners method
    assert(one.horner(2)==55);
    assert(two.horner(3)==26);
    assert(three.horner(1)==-3);

    vector<double> six_roots = six.findRoots();
    assert(six_roots[0] == -2);
    assert(six_roots[1] == -3);

    vector<double> seven_roots = seven.findRoots();
    assert(seven_roots[0] == 3);
    assert(seven_roots[1] == -0.5);



    cout << "tests work" << endl;
}