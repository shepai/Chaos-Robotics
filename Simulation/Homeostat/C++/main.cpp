#include <stdio.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>

std::random_device rd;
std::default_random_engine generator(rd()); // rd() provides a random seed
std::uniform_real_distribution<double> distribution(-1,1);

class Array
{
    float* array_;
    int head=0;
    int size=0;
    public:
        Array(int expected_size)
        {
            array_ = new float[expected_size];
            size=expected_size;
        }
        void add(float val)
        {
            //add items to head
            if(head>=size)
            {
                //change size
                float* new_ = new float[size*2]; //double size
                for(int i=0;i<size;i++)
                {
                    new_[i]=array_[i];
                }
                array_=new_;
                size=size*2;
            }
            array_[head]=val;
            head+=1;
        }
        void change(int index,float val)
        {
            //change the value to edit
            array_[index]=val;
        }
        float get(int index)
        {
            return array_[index];
        }
        int get_size()
        {
            return head;
        }
        float getLast()
        {
            return array_[head-1];
        }
};



class Unit
{
    int m,k,l,p,q,t,timer,self_ind=0;
    float* weights_set;
    int test_interval=0;
    int upper_limit,lower_limit,upper_viability,lower_viability=0;
    bool testing;
    Unit* units;
    Array weights;
    Array thetas,theta_dots,theta_dotsdots;
    public:
        void init_Unit(int test_interval, 
        int upper_viability, 
        int lower_viability, 
        int upper_limit=std::numeric_limits<int>::max(),
        int lower_limit=std::numeric_limits<int>::min(),
        int m=1,int k=1, int l=1, int p=2, int q=1,
        float theta0=2.0, float theta_dot0=0.0, float* weights_set=NULL) 
        {     // Constructor
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
            //viability
            this->upper_viability=upper_viability;
            this->lower_viability=lower_viability;

            thetas=Array(1);
            thetas.add(theta0);
            theta_dots=Array(1);
            theta_dots.add(theta_dot0);
            theta_dotsdots = Array(1);
            theta_dotsdots.add(0);
            Array units = Array(1);
            weights=Array(1);
            testing=false;
            self_ind=1; //set the units feedback
            Array test_times=Array(1);
            timer=0;
        }
        //step unit
        float step(float dt)
        {
            //get weighted sum of inputs
            float input_sum=update_inputs(dt);
            //integrate the system's dynamics
            integrate(dt, input_sum);

            if(!test_viability())
            {
                if (! testing)
                {
                    adjust_weights(dt);
                    // start testing
                    testing=true;
                }
            }

            return thetas.getLast();
        }
        //get inputs from all connect units, including this unit
        float update_inputs(float dt)
        {
            //calculate weighted sum of inputs from all connected Units (including feedback from this Unit)
            float input_sum=0.0;
            int count=0;
            Array inputs=Array(sizeof(units));
            for(int i=0;i<sizeof(units);i++)
            {
                inputs.add(units[i].get_theta() * weights.get(i));
                input_sum+= inputs.getLast(); //get last item
            }
            return input_sum;
        }
        float get_theta()
        {
            return thetas.get(thetas.get_size()-1); //get last added
        }
        //integrate system's dynamics
        void integrate(float dt, float input_sum)
        {
            //integrate the system
            float theta__ = k*theta_dots.getLast() + (l * (p-q) * input_sum);
            float theta_ = theta_dots.getLast() + (theta_dotsdots.getLast()*dt);
            float theta = thetas.getLast() + theta_dots.getLast()*dt; 

            //enforce the limits 
            if (theta>upper_limit)
            {
                theta=upper_limit;
                theta_=0;
            }else if(theta<lower_limit)
            {
                theta=lower_limit;
                theta_=0;
            }
            //store system variables
            thetas.add(theta);
            theta_dots.add(theta_);
            theta_dotsdots.add(theta__);

        }
        //test to see whether the system's essential variable (theta) is
        //within the chosen limits for viability
        bool test_viability()
        {
            if((thetas.getLast()>upper_viability) ||
            (thetas.getLast()<lower_viability))
            {
                return false;
            }
            return true;
        }
        void adjust_weights(float dt)
        //a function to randomise a Unit's weights
        {
            //implement random selector
            for(int i=0; i<weights.get_size();i++)
            {
                if((self_ind)&&i==self_ind)
                {
                    weights.change(i,-1*abs(weights.get(i)+distribution(generator)));
                }else{
                    weights.change(i,distribution(generator));
                }
                
            }
            
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
    }*/
    
    double number = distribution(generator);
    std::cout<<number;
}