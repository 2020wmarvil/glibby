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

    py::class_<glibby::Point<float, 1>>(m, "Point1")
        .def(py::init<float>());
    py::class_<glibby::Point<float, 2>>(m, "Point2")
        .def(py::init<float>());
    py::class_<glibby::Point<float, 3>>(m, "Point3")
        .def(py::init<float>());

    py::class_<glibby::Point<double, 1>>(m, "Point1D")
        .def(py::init<double>());
    py::class_<glibby::Point<double, 2>>(m, "Point2D")
        .def(py::init<double>());
    py::class_<glibby::Point<double, 3>>(m, "Point3D")
        .def(py::init<double>());
    
    // NOT WORKING
    
    m.def("ManhattanDistance1D", glibby::ManhattanDistance<float, 1>, R"pbdoc(
        Manhattan distance between 2 1D points
    )pbdoc");
    m.def("ManhattanDistance1D", glibby::ManhattanDistance<double, 1>, R"pbdoc(
        Manhattan distance between 2 1D points
    )pbdoc");
    /*m.def("ManhattanDistance1D", glibby::ManhattanDistance<uint32_t, 1>, R"pbdoc(
        Manhattan distance between 2 1D points
    )pbdoc"); */

    m.def("ManhattanDistance2D", glibby::ManhattanDistance<float, 2>, R"pbdoc(
        Manhattan distance between 2 2D points
    )pbdoc");
    m.def("ManhattanDistance2D", glibby::ManhattanDistance<double, 2>, R"pbdoc(
        Manhattan distance between 2 2D points
    )pbdoc");
    /*m.def("ManhattanDistance2D", glibby::ManhattanDistance<uint32_t, 2>, R"pbdoc(
        Manhattan distance between 2 2D points
    )pbdoc");*/

    m.def("ManhattanDistance3D", glibby::ManhattanDistance<float, 3>, R"pbdoc(
        Manhattan distance between 2 3D points
    )pbdoc");
    m.def("ManhattanDistance3D", glibby::ManhattanDistance<double, 3>, R"pbdoc(
        Manhattan distance between 2 3D points
    )pbdoc");
    /*m.def("ManhattanDistance3D", glibby::ManhattanDistance<uint32_t, 3>, R"pbdoc(
        Manhattan distance between 2 3D points
    )pbdoc");*/


    m.def("EuclideanDistance1D", glibby::EuclideanDistance<float, 1>, R"pbdoc(
        Euclidean distance between 2 1D points
    )pbdoc");
    m.def("EuclideanDistance1D", glibby::EuclideanDistance<double, 1>, R"pbdoc(
        Euclidean distance between 2 1D points
    )pbdoc");
    /*m.def("EuclideanDistance1D", glibby::EuclideanDistance<uint32_t, 1>, R"pbdoc(
        Euclidean distance between 2 1D points
    )pbdoc");*/

    m.def("EuclideanDistance2D", glibby::EuclideanDistance<float, 2>, R"pbdoc(
        Euclidean distance between 2 2D points
    )pbdoc");
    m.def("EuclideanDistance2D", glibby::EuclideanDistance<double, 2>, R"pbdoc(
        Euclidean distance between 2 2D points
    )pbdoc");
    /*m.def("EuclideanDistance2D", glibby::EuclideanDistance<uint32_t, 2>, R"pbdoc(
        Euclidean distance between 2 2D points
    )pbdoc");*/

    m.def("EuclideanDistance3D", glibby::EuclideanDistance<float, 3>, R"pbdoc(
        Euclidean distance between 2 3D points
    )pbdoc");
    m.def("EuclideanDistance3D", glibby::EuclideanDistance<double, 3>, R"pbdoc(
        Euclidean distance between 2 3D points
    )pbdoc");
    /*m.def("EuclideanDistance3D", glibby::EuclideanDistance<uint32_t, 3>, R"pbdoc(
        Euclidean distance between 2 3D points
    )pbdoc");*/
    
    // NOT WORKING 
    
        

}
