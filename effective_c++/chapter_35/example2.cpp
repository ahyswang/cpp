#include <iostream>
#include <functional>
/*
tr1::function 完成Strategy模式

g++ example2.cpp -std=c++11

参考：
1、Strategy模式（策略模式）
*/
class GameCharacter;
int defaultHealthCalc(const GameCharacter& gc);
// 游戏人物
class GameCharacter {
public:
    //HealthCalcFunc可以是任何（可调用物），可被调用并接受
    //任何兼容于GameCharacter之物，返回任何兼容于int的东西。
    typedef std::function<int (const GameCharacter&)> HealthCalcFunc;
    explicit GameCharacter(HealthCalcFunc hcf = defaultHealthCalc)
        : healthFunc(hcf)
    {}
    int healthValue() const                                 //drived class 不重新定义。 
    {
        return healthFunc(*this);
    }
private:
    HealthCalcFunc healthFunc;
};

short calcHealth(const GameCharacter&)  //健康计算函数
{                                       //注意其返回值类型为non-int
    return 100;
}

struct HealthCalculator {               //为计算健康而设计的函数对象
    int operator() (const GameCharacter&) const 
    {
        return 90;
    }
};

class GameLevel {                   
public:
    float health(const GameCharacter&) const    //成员函数，用以计算健康，注意返回类型是non-int 
    {
        return 50;
    } 
};

class EyeCandyCharacter : public GameCharacter {
public:
    explicit EyeCandyCharacter(HealthCalcFunc hcf = defaultHealthCalc)
        : GameCharacter(hcf)
    {}
};

class EvilBadGuy : public GameCharacter {
public:
    explicit EvilBadGuy(HealthCalcFunc hcf = defaultHealthCalc)
        : GameCharacter(hcf)
    {}
};

int main()
{

    GameCharacter ebg1(calcHealth); //人物1:使用函数
    GameCharacter ebg2(HealthCalculator()); //人物2:使用函数对象
    GameLevel currentLevel; //人物3:使用成员函数
    EvilBadGuy ebg3(std::bind(&GameLevel::health, currentLevel, std::placeholders::_1)); //GameLevel::health 第一个参数是this

    std::cout << ebg1.healthValue() << std::endl;
    //TODO:
    //std::cout << ebg2.healthValue() << std::endl;
    std::cout << ebg3.healthValue() << std::endl;

    return 0;
}