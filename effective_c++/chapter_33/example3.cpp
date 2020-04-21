#include <iostream>


//2
class Base {
    private :
    int x;
    public:
        virtual void mf1() = 0;
        virtual void mf1(int){};//
        virtual void mf2(){};
        void mf3(){};
        void mf3(double){}; //
};

class Drived : public Base {
public:
    using Base::mf1;  //让Base class内名为mf1和mf3的所有东西在Drived作用域都可见（并且public）
    using Base::mf3;
    virtual void mf1(){};
    void mf3(){}; //
    void mf4(){}; 
} ;

int main()
{
    Drived d;
    int x;
    d.mf1();   //ok  Driver::mf1
    d.mf1(x);  //err Base::mf1
    d.mf2();   //ok  Base::mf2
    d.mf3();   //ok  Driver::mf3
    d.mf3(x);  //err Base::mf3
    return 0;
}