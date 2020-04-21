#include <iostream>


class Person 
{
public:
    Person ()
    {
        std::cout << "Persion ctor" << std::endl;
    }
    ~Person ()
    {
        std::cout << "Persion dtor" << std::endl;
    }
    void show() 
    {
        std::cout << "Persion show" << std::endl;
    }
};

int main(int argc, char** argv)
{   
    Person* personArray = new Person[5];
    //delete personArray; //删除一个对象 会导致删除非法的内存
    delete[] personArray; //删除一组对象
    return 0;
}