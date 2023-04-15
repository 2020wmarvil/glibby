#include "glibby/primitives/point.h"
#include "pybind11/pybind11.h"

#include <iostream>

void PrintDistance()
{
	using namespace glibby;
	std::cout << EuclideanDistance(Point2({ 0, 0 }), Point2({ 1, 0 })) << std::endl;
}

int add(int i, int j) {
    return i + j;
}

namespace py = pybind11;

PYBIND11_MODULE(glibby, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: glibby

        .. autosummary::
           :toctree: _generate

           add
           subtract
           printDistance
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers
    )pbdoc");

    m.def("printDistance", &PrintDistance, R"pbdoc(
        Demo function to print euclidean distance between two hard coded points
    )pbdoc");
}
