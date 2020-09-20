#include <iostream>
#include "sounds.h"

/*This is the Cuda function, that does special Cuda things*/
__global__
void ThisIsACudaFunction(){
    
};

extern "C" {
    void Wrapper::StandardFunction(){
        std::cout << "GPU IS DOING THIS" << std::endl;
        /*This function is a "wrapper" for the Cuda function above. Cuda functions need to be called with special thingies that won't compile in a normal C++ file*/
        ThisIsACudaFunction<<<1,1>>>();
    };
}