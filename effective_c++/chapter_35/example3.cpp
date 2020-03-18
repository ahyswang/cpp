#include <iostream>

/*
古典的Strategy模式
*/

class GameCharacter;  //前置申明
// 健康计算
class HealthCalcFunc {
public:
    virtual int calc(const GameCharacter& gc) const 
    {
        std::cout << "HealthCalcFunc::calc" << std::endl;
        return 100;
    }
};

// 游戏人物
class GameCharacter {
public:
    explicit GameCharacter(HealthCalcFunc* phcf)
        : pHealthCalc(phcf)
    {}
    // 计算人物健康指数
    int healthValue() const                                 //drived class 不重新定义。 
    {
        return pHealthCalc->calc(*this);
    }
private:
    HealthCalcFunc * pHealthCalc;
};

class EyeCandyCharacter : public GameCharacter {
public:
    explicit EyeCandyCharacter(HealthCalcFunc* phcf)
        : GameCharacter(phcf)
    {}
};

class EvilBadGuy : public GameCharacter {
public:
    explicit EvilBadGuy(HealthCalcFunc* phcf)
        : GameCharacter(phcf)
    {}
};

class SlowHealthLoser : public HealthCalcFunc 
{
public:
    virtual int calc(const GameCharacter& gc) const 
    {
        std::cout << "SlowHealthLoser::calc" << std::endl;
        return 50;
    }
};

class FastHealthLoser : public HealthCalcFunc
{
public:
    virtual int calc(const GameCharacter& gc) const 
    {
        std::cout << "FastHealthLoser::calc" << std::endl;
        return 200;
    }
};

int main()
{
    HealthCalcFunc hcf1;
    SlowHealthLoser hcf2;
    FastHealthLoser hcf3;

    GameCharacter ebg1(&hcf1);
    EyeCandyCharacter ebg2(&hcf2);
    EvilBadGuy ebg3(&hcf3);

    ebg1.healthValue();
    ebg2.healthValue();
    ebg3.healthValue();

    return 0;
}