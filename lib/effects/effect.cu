#include <iostream>
#include "effect.h"

/*This is the Cuda function, that does special Cuda things*/
__global__
void ThisIsACudaFunction(){
    int i = 1 + 1;
};

/*This function is a "wrapper" for the Cuda function above. Cuda functions need to be called with special thingies that won't compile in a normal C++ file*/
void DynamicSounds::StandardFunction(){
    std::cout << "GPU IS [not] DOING THIS" << std::endl;
    
    ThisIsACudaFunction<<<1,1>>>();
};