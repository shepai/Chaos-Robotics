//#include <pybind11/pybind11.h>

#include <iostream>

namespace BVP
{
    class BVP
    {
    public:
        //store cordinae values
        struct coords
        {
            double x;
            double y;
        };
        double* gen(int size, int c, int a, float dt);
    private:
        coords get(double x, double y, int c, int a, float dt);

    };
}
BVP::BVP::coords BVP::BVP::get(double x, double y, int c, int a, float dt)
{
    double x_d = 0.0, xs = 0.0, ys = 0.0;
    x_d = c * (x - ((std::pow(x, 3)) / 3) + y);
    xs = (c * (1 - std::pow(x, 2)) * x_d) - x;
    ys = -x * a;
    //create coordinates to return
    BVP::BVP::coords res = { x + (xs * dt),y + (ys * dt) };
    return res;
}
// Returns a + b
double* BVP::BVP::gen(int size, int c, int a, float dt)
{
    double* arr = new double[size * 2]; //generate an array of size x and y
    arr[0] = 0.0;
    arr[1] = 1.0;
    BVP::BVP::coords cord;
    for (int i = 0; i < size - 2; i += 2) //generate array
    {
        cord = BVP::BVP::get(arr[i], arr[i + 1], c, a, dt);
        arr[i + 2] = cord.x;
        arr[i + 3] = cord.y;
    }
    return arr;
}


int main()
{
    BVP::BVP obj;
    double* ar = obj.gen(100,1,2,0.01);
    for (int i = 0; i < 100; i += 2) //generate array
    {
        std::cout << ar[i] << ", " << ar[i + 1];
    }
}
