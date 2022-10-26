#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <stdio.h>
#include <cmath>
#include <iostream>


class BVP
{
public:
    //store cordinae values
    struct coords
    {
        float x;
        float y;
    };
    float* gen(int size, float c, float a, float dt);
private:
    coords get(float x, float y, float c, float a, float dt);

};
BVP::coords BVP::get(float x, float y, float c, float a, float dt)
{
    float x_d = 0.0, xs = 0.0, ys = 0.0;
    x_d = c * (x - ((std::pow(x, 3)) / 3) + y);
    xs = (c * (1 - std::pow(x, 2)) * x_d) - x;
    ys = -x * a;
    //create coordinates to return
    BVP::coords res = { x + (xs * dt),y + (ys * dt) };
    return res;
}
// Returns a + b
float* BVP::gen(int size,  float c, float a, float dt)
{
    float* arr = new float[size * 2]; //generate an array of size x and y
    arr[0] = 0.0;
    arr[1] = 1.0;
    BVP::coords cord;
    for (int i = 2; i < size * 2; i += 2) //generate array
    {
        
        cord = BVP::get(arr[i-2], arr[i -1], c, a, dt);
        //printf("2%f,%f\n",cord.x,cord.y);
        arr[i + 0] = cord.x;
        arr[i + 1] = cord.y;
    }
    return arr;
}

namespace py = pybind11;

PYBIND11_MODULE(bvp, m) { //loercase for python modules
    py::class_<BVP>(m, "BVP")
        .def(py::init<>())
        .def("gen",
            [](BVP& bvp, int size,  float c, float a, float dt)
            {
                return py::memoryview::from_memory(bvp.gen(size, c, a, dt), size * 2 * sizeof(float));
            }); //only public functions
}
/*
int main()
{
    BVP::BVP obj;
    float* ar = obj.gen(100,1,2,0.01);
    for (int i = 0; i < 100; i += 2) //generate array
    {
        std::cout << ar[i] << ", " << ar[i + 1];
    }
}*/