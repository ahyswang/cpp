#include <iostream>

/*
Non－Virtual Interface手法实现Template Method模式

Template Method模式是Base Class定义执行流程，Drived Class实现具体的流程细节。
参考：
1、Template Method模式（模版设计模式）
https://www.cnblogs.com/dgwblog/p/8968891.html
*/

// 游戏人物
class GameCharacter {
public:
    // 计算人物健康指数
    int healthValue() const                                 //drived class 不重新定义。 
    {
        std::cout << "doHealthValue before" << std::endl;   //做一些事前工作。
        int retVal = doHealthValue();                       //做真正的工作。
        std::cout << "doHealthValue after" << std::endl;    //做一些时候工作。
        return retVal;
    }
private:
    virtual int doHealthValue() const                       //drived class 可重新定义它。
    {
        std::cout << "GameCharacter::doHealthValue" << std::endl;
        return 100;
    }
};

class EyeCandyCharacter : public GameCharacter {
private:
    virtual int doHealthValue() const 
    {
        std::cout << "EyeCandyCharacter::doHealthValue" << std::endl;
        return 90;
    }
};

class EvilBadGuy : public GameCharacter {
private:
    virtual int doHealthValue() const 
    {
        std::cout << "EvilBadGuy::doHealthValue" << std::endl;
        return 50;
    }
};

int main()
{
    GameCharacter ebg1;
    EyeCandyCharacter ebg2;
    EvilBadGuy ebg3;

    ebg1.healthValue();
    ebg2.healthValue();
    ebg3.healthValue();

    return 0;
}