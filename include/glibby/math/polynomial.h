#include<vector>
#include <cmath>
using namespace std;

namespace glibby
{
    class Quadratic{

        public:
            //constructor
            Quadratic(vector<int>_values){
                values =_values;
            }

            // copy constructor
            Quadratic(const Quadratic &one){
                for (int i = 0; i < one.get_values().size(); i++){
                    values.push_back(one.get_values()[i]);
                }
            }

            // add function
            Quadratic add(const Quadratic &two) const {
                vector<int> data2 = two.get_values();
                int size1 = values.size(), size2 = data2.size();
                int max_size = max(size1, size2);
                vector<int> result(max_size);

                for (int i = 0; i < max_size; i++) {
                    result[i] = (i < size1 ? values[i] : 0) + (i < size2 ? data2[i] : 0);
                }

                return Quadratic(result);
            }

            //subtract function
            Quadratic subtract(const Quadratic &two){
                vector<int> result;
                vector<int> data2 = two.get_values();
                int max_size = max(values.size(), data2.size());
                result.resize(max_size);

                for (int i = 0; i < max_size; i++){
                    if (i < values.size() && i < data2.size()){
                        result[i] = values[i] - data2[i];
                    } else if (i < values.size()){
                        result[i] = values[i];
                    } else {
                        result[i] = data2[i];
                    }
                }
                Quadratic answer(result);
                return answer;
            }

            //returns the values
            vector<int> get_values() const {
                return values;
            }

            //evaluates the object at a given point
            int evaluate(int val){
                int product = values[0];
                for (int i = 1; i < values.size(); i++){
                    product = product + (values[i] * pow(val,i));
                }
                return product;
            }

            // horners method is a faster way of evalutaing polynomials
            int horner(int x) {
                int result = values.back();
                for (int i = values.size() - 2; i >= 0; i--) {
                    result = result * x + values[i];
                }
                return result;
            }

            //multipliying function
            Quadratic multiply(const Quadratic &one) {
                int n1 = values.size();
                int n2 = one.get_values().size();
                int n = n1 + n2 - 1;
                std::vector<int> result(n, 0);
                for (int i = 0; i < n1; i++) {
                    for (int j = 0; j < n2; j++) {
                        result[i + j] += values[i] * one.get_values()[j];
                    }
                }
                return Quadratic(std::move(result));
            }

            // finds the roots of the given polynomial
            vector<double> findRoots() const {
                vector<double> roots;
                const double& a = values[2];
                const double& b = values[1];
                const double& c = values[0];

                const double determinant = b * b - 4 * a * c;

                if (determinant >= 0) {
                    const double root_d = sqrt(determinant);
                    roots.push_back((-b + root_d) / (2 * a));
                    if (determinant > 0) {
                        roots.push_back((-b - root_d) / (2 * a));
                    }
                }

                return roots;
            }

            // overloading - operator 
            Quadratic operator-(const Quadratic& b) {
                Quadratic tmp(values);
                Quadratic answer = tmp.subtract(b);
                return answer;
            }

            // overloading + operator 
            Quadratic operator+(const Quadratic& b) {
                Quadratic tmp(values);
                Quadratic answer = tmp.add(b);
                return answer;
            }

            // overloading * operator
            Quadratic operator*(const Quadratic& b) {
                Quadratic tmp(values);
                Quadratic answer = tmp.multiply(b);
                return answer;
            }

            // overloading = operator
            Quadratic operator=(const Quadratic& b) {
                return Quadratic(b);
            }

            // bool == operator
            bool operator==(const Quadratic& one) const {
                for (int i = 0; i < one.get_values().size(); i++){
                    if (values[i] != one.get_values()[i]){
                        return false;
                    }
                }
                return true;
            }

             // bool != operator
            bool operator!=(const Quadratic& one) const {
                for (int i = 0; i < one.get_values().size(); i++){
                    if (values[i] != one.get_values()[i]){
                        return true;
                    }
                }
                return false;
            }

        private:
            vector<int> values;
    };
}