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
            for (int i = 0; i < size; i++)
            {
                new_[i] = array_[i];
            }
            size = newSize;
            array_ = new float[size];
            std::copy(new_, new_, array_);
        }
        else {
            array_[head] = val;
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
        return head;
    }
    float getLast()
    {
        return array_[head - 1];
    }
    float* getAll()
    {
        return array_;
    }
};