#include <iostream>

class Base 
{

};

class Drived : public Base 
{

};

int main()
{
    Drived d;
    Base* pb = &d;
    std::cout << (void *)pb << std::endl;
    std::cout << &d << std::endl;
    return 0;
}