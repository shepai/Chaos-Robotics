#include <iostream>
class Array
{
    float* array_;
    int head = 0;
    int size = 0;
public:
    Array() = default;
    Array(int expected_size) //create array based n expected initial size 
    {
        //std::cout << expected_size << "\n";
        array_ = new float[expected_size];
        size = expected_size;
        head = 0;
    }
    void add(float val)
    {
        
        //add items to head
        if (head >= size)
        {
            //change size
            int newSize = size * 2;
            float* new_ = new float[newSize]; //double size
            for (int i = 0; i < head; i++)
            {
                
                new_[i] = array_[i];
            }
            new_[head] = val;
            size = newSize;
            array_ = new float[size];
            //copy back over
            for (int i = 0; i < head+1; i++)
            {

                array_[i]= new_[i];
            }
            
        }
        else {
            array_[head] = val;
            //std::cout << "\nWhat we add: " << array_[head] << "\n";
        }
        head = head + 1;
    }
    void change(int index, float val)
    {
        //change the value to edit
        array_[index] = val;
    }
    float get(int index)
    {
        return array_[index];
    }
    int get_size()
    {
        return head-1;
    }
    float getLast()
    {
        return array_[head - 1];
    }
    float* getAll()
    {
        float* values = new float[head-1];
        for (int i = 0; i < head-1; i++)
        {
            
            values[i] = array_[i];
        }
        return values;
    }
};