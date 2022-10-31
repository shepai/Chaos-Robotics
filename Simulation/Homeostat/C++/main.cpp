#include <stdio.h>
#include <cmath>
#include <iostream>
#include <limits>
class BVP
{
    public:
    //store cordinae values
    struct coords
    {
        double x;
        double y;
    };
    double* gen(int size, int c, int a, double dt);
private:
    coords get(double x, double y, int c, int a, double dt);

};
BVP::coords BVP::get(double x, double y, int c, int a, double dt)
{
    double x_d = 0.0, xs = 0.0, ys = 0.0;
    x_d = c * (x - ((std::pow(x, 3)) / 3) + y);
    xs = (c * (1 - std::pow(x, 2)) * x_d) - x;
    ys = -x * a;
    //create coordinates to return
    BVP::coords res = { x + (xs * dt),y + (ys * dt) };
    return res;
}
// Returns a + b
double* BVP::gen(int size, int c, int a, double dt)
{
    double* arr = new double[size * 2]; //generate an array of size x and y
    arr[0] = 0.0;
    arr[1] = 1.0;
    BVP::coords cord;
    for (int i = 0; i < size - 2; i += 2) //generate array
    {
        cord = BVP::get(arr[i], arr[i + 1], c, a, dt);
        arr[i + 2] = cord.x;
        arr[i + 3] = cord.y;
    }
    return arr;
}




class Homeostat
{
    int m,k,l,p,q,t=0;
    float* weights_set;
    int test_interval=0;
    int upper_limit,lower_limit=0;
    public:
        Homeostat(int test_interval, 
        int upper_viability, 
        int lower_viability, 
        int upper_limit=std::numeric_limits<int>::max(),
        int lower_limit=std::numeric_limits<int>::min(),
        int m=1,int k=1, int l=1, int p=2, int q=1,
        int theta0=2, int theta_dot0=0, float* weights_set=NULL) {     // Constructor
            //set weights
            this->weights_set=weights_set;
            //minimum time to wait between changing the weights
            this->test_interval=test_interval;
            //clock
            this->t=t;
            //system equation parameters
            this->m=m;
            this->k=k;
            this->l=l;
            this->p=p;
            this->q=q;
            //upper limit and lower limit
            this->upper_limit=upper_limit;
            this->lower_limit=lower_limit;
            //upper viability

        }
};

int main()
{
    BVP obj;
    double* ar = obj.gen(100,1,2,0.01);
    for (int i = 0; i < 100; i += 2) //generate array
    {
        std::cout << ar[i] << ", " << ar[i + 1];
    }
}