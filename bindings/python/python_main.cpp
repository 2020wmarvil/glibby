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


template<typename T>
T ManhattanDistance1D(const glibby::Point<T, 1>& p1, const glibby::Point<T, 1>& p2)
{
    return glibby::ManhattanDistance(p1, p2);
}
template<typename T>
T ManhattanDistance2D(const glibby::Point<T, 2>& p1, const glibby::Point<T, 3>& p2)
{
    return glibby::ManhattanDistance(p1, p2);
}
template<typename T>
T ManhattanDistance3D(const glibby::Point<T, 3>& p1, const glibby::Point<T, 3>& p2)
{
    return glibby::ManhattanDistance(p1, p2);
}

template<typename T>
T EuclideanDistance1D(const glibby::Point<T, 1>& p1, const glibby::Point<T, 1>& p2)
{
    return glibby::EuclideanDistance(p1, p2);
}
template<typename T>
T EuclideanDistance2D(const glibby::Point<T, 2>& p1, const glibby::Point<T, 2>& p2)
{
    return glibby::EuclideanDistance(p1, p2);
}
template<typename T>
T EuclideanDistance3D(const glibby::Point<T, 3>& p1, const glibby::Point<T, 3>& p2)
{
    return glibby::EuclideanDistance(p1, p2);
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

    py::class_<glibby::Point<float, 1>>(m, "Point1")
        .def(py::init<float&, int&>());
    py::class_<glibby::Point<float, 2>>(m, "Point2")
        .def(py::init<float&, int&>());
    py::class_<glibby::Point<float, 3>>(m, "Point3")
        .def(py::init<float&, int&>());
    
    // NOT WORKING
    
    m.def("ManhattanDistance1D", ManhattanDistance1D<float>, R"pbdoc(
        Manhattan distance between 2 1D points
    )pbdoc");
    m.def("ManhattanDistance1D", ManhattanDistance1D<double>, R"pbdoc(
        Manhattan distance between 2 1D points
    )pbdoc");
    m.def("ManhattanDistance1D", ManhattanDistance1D<uint32_t>, R"pbdoc(
        Manhattan distance between 2 1D points
    )pbdoc");

    m.def("ManhattanDistance2D", ManhattanDistance2D<float>, R"pbdoc(
        Manhattan distance between 2 2D points
    )pbdoc");
    m.def("ManhattanDistance2D", ManhattanDistance2D<double>, R"pbdoc(
        Manhattan distance between 2 2D points
    )pbdoc");
    m.def("ManhattanDistance2D", ManhattanDistance2D<uint32_t>, R"pbdoc(
        Manhattan distance between 2 2D points
    )pbdoc");

    m.def("ManhattanDistance3D", ManhattanDistance3D<float>, R"pbdoc(
        Manhattan distance between 2 3D points
    )pbdoc");
    m.def("ManhattanDistance3D", ManhattanDistance3D<double>, R"pbdoc(
        Manhattan distance between 2 3D points
    )pbdoc");
    m.def("ManhattanDistance3D", ManhattanDistance3D<uint32_t>, R"pbdoc(
        Manhattan distance between 2 3D points
    )pbdoc");


    m.def("EuclideanDistance1D", EuclideanDistance1D<float>, R"pbdoc(
        Euclidean distance between 2 1D points
    )pbdoc");
    m.def("EuclideanDistance1D", EuclideanDistance1D<double>, R"pbdoc(
        Euclidean distance between 2 1D points
    )pbdoc");
    m.def("EuclideanDistance1D", EuclideanDistance1D<uint32_t>, R"pbdoc(
        Euclidean distance between 2 1D points
    )pbdoc");

    m.def("EuclideanDistance2D", EuclideanDistance2D<float>, R"pbdoc(
        Euclidean distance between 2 2D points
    )pbdoc");
    m.def("EuclideanDistance2D", EuclideanDistance2D<double>, R"pbdoc(
        Euclidean distance between 2 2D points
    )pbdoc");
    m.def("EuclideanDistance2D", EuclideanDistance2D<uint32_t>, R"pbdoc(
        Euclidean distance between 2 2D points
    )pbdoc");

    m.def("EuclideanDistance3D", EuclideanDistance3D<float>, R"pbdoc(
        Euclidean distance between 2 3D points
    )pbdoc");
    m.def("EuclideanDistance3D", EuclideanDistance3D<double>, R"pbdoc(
        Euclidean distance between 2 3D points
    )pbdoc");
    m.def("EuclideanDistance3D", EuclideanDistance3D<uint32_t>, R"pbdoc(
        Euclidean distance between 2 3D points
    )pbdoc");
    
    // NOT WORKING 
    
        

}
