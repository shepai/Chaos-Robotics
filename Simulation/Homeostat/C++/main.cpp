#include <stdio.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <algorithm>
#include <iterator>
#include "array.cpp"

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
    Array thetas, theta_dots, theta_dotsdots;
public:
    Unit() = default;

    void init_Unit(int test_interval,
        int upper_viability,
        int lower_viability,
        int upper_limit = std::numeric_limits<int>::max(),
        int lower_limit = std::numeric_limits<int>::min(),
        int m = 1, int k = 1, int l = 1, int p = 2, int q = 1,
        float theta0 = 2.0, float theta_dot0 = 0.0, float* weights_set = NULL)
    {     // Constructor
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
        t += dt;
        timer += dt;
        return thetas.getLast();
    }
    
    float get_theta()
    {
        int pin = thetas.get_size();
        return thetas.get(pin-1); //get last added
    }
    //integrate system's dynamics
    void integrate(float dt, float input_sum)
    {
        //integrate the system
        float theta__ = k * theta_dots.getLast() + (l * (p - q) * input_sum);
        float theta_ = theta_dots.getLast() + (theta_dotsdots.getLast() * dt);
        float theta = thetas.getLast() + theta_dots.getLast() * dt;

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
    void adjust_weights(float dt)
        //a function to randomise a Unit's weights
    {
        //implement random selector
        for (int i = 0; i < weights.get_size(); i++)
        {
            if ((self_ind) && i == self_ind)
            {
                weights.change(i, -1 * abs(weights.get(i) + distribution(generator)));
            }
            else {
                weights.change(i, distribution(generator));
            }

        }

    }
    void add_connections(Unit* unit)
    {
        //units = new Unit[sizeof(unit)];
        units = unit;
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
    void __init__(int n_units, int upper_limit, int lower_limit,
        int upper_viability, int lower_viability, float* weights_set = NULL,
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
                2.0, 0.0, weights_set);
            
        }

        //initialise()
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
    float* getUnit(int i=0)
    {
        return units[i].getTheta();
    }
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
        
        for (int i = 0; i < n_items; i++)
        {
            
            float calc = units[i].get_theta(); //* weights.get(i);
            inputs.add(calc);
            inputSum += inputs.getLast(); //get last item

        }

        return inputSum;
    }
};

 

int main()
{

    /*
    std::cout<<"start\n";
    Array arr = Array(1);
    for(int i=0;i<10000;i++)
    {
        arr.add(i);
    }
    */ 
     
    int n_units = 1;
    int upper_limit = 20;
    int lower_limit = -20;
    float upper_viability = 1;
    float lower_viability = -1;
    float* weights_set = NULL;
    int test_interval = 10;

    float dt = 0.005;
    int duration = 10; 
    int count = 0;
    float t = 0;
     
    //create homeostat
    Homeostat hom = Homeostat();
    hom.__init__(n_units, upper_limit, lower_limit, upper_viability, lower_viability, weights_set, test_interval);

    while (t < duration) //main loop
    {
        hom.step(dt);
        t += dt;
        count++;
    }
    std::cout << "Done...." << count;
    float* vals = hom.getUnit(0); 
    int truesize = hom.getUnitThetaSize(0);
    printf("\nSize is %d\n", truesize);
    std::cout << "\n>" << vals[0];
    for (int i = 0; i < truesize; i++)
    {
        std::cout << "\n>" << vals[i];
    }
      
    /**/


}