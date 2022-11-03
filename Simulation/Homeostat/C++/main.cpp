#include <stdio.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <algorithm>
#include <iterator>
#include "array.cpp"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

std::random_device rd;
std::default_random_engine generator(rd()); // rd() provides a random seed
std::uniform_real_distribution<double> distribution(-1, 1);

class Unit
{
    int m, k, l, p, q, self_ind = 0;
    float t, timer = 0.0;
    float* weights_set;
    int test_interval = 0;
    int upper_limit, lower_limit, upper_viability, lower_viability = 0;
    bool testing;
    Unit* units;
    Array weights;
    int units_num=0;
    Array thetas, theta_dots, theta_dotsdots;
public:
    Unit() = default;

    void init_Unit(int test_interval,
        int upper_viability,
        int lower_viability,
        int upper_limit = std::numeric_limits<int>::max(),
        int lower_limit = std::numeric_limits<int>::min(),
        int m = 1, int k = 1, int l = 1, int p = 2, int q = 1,
        float theta0 = 2.0, float theta_dot0 = 0.0)
    {     // Constructor
        float* weights_set = NULL;
        //set weights
        this->weights_set = weights_set;
        //minimum time to wait between changing the weights
        this->test_interval = test_interval;
        //clock
        this->t = t;
        //system equation parameters
        this->m = m;
        this->k = k;
        this->l = l;
        this->p = p;
        this->q = q;
        //upper limit and lower limit
        this->upper_limit = upper_limit;
        this->lower_limit = lower_limit;
        //viability
        this->upper_viability = upper_viability;
        this->lower_viability = lower_viability;
        thetas = Array(1);
        thetas.add(theta0);
        theta_dots = Array(1);
        theta_dots.add(theta_dot0);
        theta_dotsdots = Array(1);
        theta_dotsdots.add(0);
        units = new Unit[0];
        //weights=Array(1);
        testing = false;
        self_ind = 1; //set the units feedback
        Array test_times = Array(1);
        timer = 0;
        timer = 0;
    }
    //step unit
    float step(float dt,float input_sum)
    {
        //get weighted sum of inputs

        //float input_sum = update_inputs(dt);
        //integrate the system's dynamics
        
        integrate(dt, input_sum);
        
        if (!test_viability())
        {
            if (!testing)
            {
                adjust_weights(dt);
                // start testing
                testing = true;
            }
        }

        if (timer > test_interval)
        {//reset timer
            timer = 0;
            testing = false;
        }
        t += dt;
        timer += dt;
        return thetas.getLast();
    }
    
    float get_theta()
    {
        //int pin = thetas.get_size();
        return thetas.getLast(); //get last added
    }
    //integrate system's dynamics
    void integrate(float dt, float input_sum)
    {
        //integrate the system
        float theta__ = (k * theta_dots.getLast()) + (l * (p - q) * input_sum);
        float theta_ = (theta_dots.getLast()) + (theta_dotsdots.getLast() * dt);
        float theta = (thetas.getLast()) +(theta_dots.getLast() * dt);

        //enforce the limits 
        if (theta > upper_limit)
        {
            theta = upper_limit;
            theta_ = 0;
        }
        else if (theta < lower_limit)
        {
            theta = lower_limit;
            theta_ = 0;
        }
        //store system variables
        
        thetas.add(theta);
        
        theta_dots.add(theta_);
        theta_dotsdots.add(theta__);
    }
    //return theta stuff
    float* getTheta()
    {
        return thetas.getAll();
    }
    //return size of theta
    int sizeTheta()
    {
        return thetas.get_size();
    }
    //test to see whether the system's essential variable (theta) is
    //within the chosen limits for viability
    bool test_viability()
    {
        if ((thetas.getLast() > upper_viability) ||
            (thetas.getLast() < lower_viability))
        {
            return false;
        }
        return true;
    }
    void adjust_weights(float dt) //a function to randomise a Unit's weights
    {
        //implement random selector
        for (int i = 0; i < weights.get_size()+1; i++)
        {
            if (self_ind>0 && i == self_ind)
            {
                weights.change(i, -1 * abs(weights.get(i) + distribution(generator)));
            }
            else {
                weights.change(i, distribution(generator));
            }

        }

    }
    void add_connections(float weight)
    {
        //units = new Unit[sizeof(unit)];
        //units = unit;
        weights.add(weight);
        self_ind = units_num;
        units_num+=1;
    }
    //gather the weights per neuron
    float* getWeights()
    {
        return weights.getAll();
    }
    //gather the weights per neuron
    Array getWeightArray()
    {
        return weights;
    }

};

//A class to represent a Homeostat, with an arbitrary number of units.

class Homeostat
{
    float t = 0;
    Unit* units;
    int n_items = 0;
public:
    Homeostat() = default;
    void init(int n_units, int upper_limit, int lower_limit,
        int upper_viability, int lower_viability,
        int test_interval = 10)
    {
        units = new Unit[n_units];
        n_items = n_units;
        //construct units

        for (int i = 0; i < n_units; i++)
        {
            units[i] = Unit();
            units[i].init_Unit(test_interval = test_interval, upper_viability,
                lower_viability, upper_limit,
                lower_limit, 1, 1, 1, 2, 1,
                2.0, 0.0);

        }
        float val;
        //initialise()
        //add a series of weights to each unit for x amunt of units
        for (int i = 0; i < n_units; i++)
        {
            for (int j = 0; j < n_units; j++)
            {
                val = distribution(generator);
                units[i].add_connections(val);
            }
        }
        
        t = 0;

    }
    
    //tep all units in the Homeostat forwards
    void step(float dt)
    {
        float sum = updateInputs();
        for (int i = 0; i<n_items; i++)
        {
            units[i].step(dt,sum);
        }

        t += dt;
    } 
    //return a float array of values generated as get theta values generated
    //@param i for the index of the unit
    //@param size for the expected size of the array, this is only relevant for the python conversion
    float* getUnit(int i=0,int size=10)
    {
        if(getUnitThetaSize(i)==0)
        {
            float* ar=new float[1];
            return ar;
        }
        return units[i].getTheta();
    }
    //return the size of the theta array
    //@param i for the index of the unit
    int getUnitThetaSize(int i = 0)
    {
        return units[i].sizeTheta();
    }
    //update the inputs from outside
    float updateInputs()
    {
        float inputSum = 0.0;
        int count = 0;
        Array inputs = Array(sizeof(units));
        //loop through all the inputs and save them
        for (int i = 0; i < n_items; i++)
        {
            //std::cout << "\nweight" << units[i].getWeightArray().get(i);
            float calc = units[i].get_theta()* units[i].getWeightArray().get(i);
            inputs.add(calc);
            inputSum += inputs.getLast(); //get last item

        }

        return inputSum;
    }
};

 namespace py = pybind11;
PYBIND11_MODULE(homeostat, m) { //loercase for python modules
    py::class_<Homeostat>(m, "Homeostat")
        .def(py::init<>())
        .def("init", &Homeostat::init, "Initialize the initial function", py::arg("n_units"), 
            py::arg("upper_limit"), py::arg("lower_limit"), py::arg("upper_viability"),py::arg("lower_viability"),
            py::arg("test_interval"))
        .def("step",&Homeostat::step,"Make a step through the event",py::arg("dt"))
        .def("updateInputs",&Homeostat::updateInputs)
        .def("getUnit",
        [](Homeostat & homeostat,int i=0,int size=10)
            {
                return py::memoryview::from_memory(homeostat.getUnit(i,size),size*sizeof(float));
            });
    py::class_<Unit>(m, "Unit")
        .def(py::init<>());
        
}

 /*
//step(float dt)
PYBIND11_MODULE(homeostat, m) { //loercase for python modules
    py::class_<Homeostat>(m, "Homeostat")
        .def(py::init<>()),
        .def("init", &init, "Initialize the initial function", py::arg("n_units"), 
        py::arg("upper_limit"), py::arg("lower_limit"), py::arg("upper_viability"),
        py::arg("weights_set"), py::arg("test_interval")),
        .def("step",&step,"Make a step through the event",py::arg("dt")),
        .def("updateInputs",[](Homeostat& homeostat)
            {
                return py::memoryview::from_memory(homeostat.updateInputs(),sizeof(int));
            },"Update the inputs and return input sim"); 
    py::class_<Unit>(m, "Unit")
        .def(py::init<>()),
        .def("getTheta",[](Unit& getTheta)
            {
                return py::memoryview::from_memory(Unit.getTheta(), Unit.sizeTheta() * sizeof(int));
            });
}
*/
/*

int main()
{
     
    int n_units = 3;
    int upper_limit = 20;
    int lower_limit = -20;
    float upper_viability = 1;
    float lower_viability = -1;
    float* weights_set = NULL;
    int test_interval = 10;

    float dt = 0.005;
    int duration = 100; 
    int count = 0;
    float t = 0;
    std::cout << "Make homeostat\n";
    //create homeostat
    Homeostat hom = Homeostat();
    hom.__init__(n_units, upper_limit, lower_limit, upper_viability, lower_viability, weights_set, test_interval);

    while (t < duration) //main loop
    {
        hom.step(dt);
        t += dt;
        count++;
    }
    
    float* vals = hom.getUnit(0); 
    int truesize = hom.getUnitThetaSize(0);
    printf("\nSize is %d\n", truesize);
    
    for (int i = 0; i < truesize; i++)
    {
        std::cout << "\n>" << vals[i];
    }
      


}*/