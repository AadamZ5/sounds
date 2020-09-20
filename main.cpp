#include <iostream>
#include "sounds-util/sounds/sounds.h"

using namespace std;

long int buf[1024];

int main (int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;
    Wrapper::StandardFunction();
    exit(0);
}

