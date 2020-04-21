#include <iostream>

class Widget 
{
public:
    Widget()
    {
        std::cout << "Widget::ctor" << std::endl;
    }
    void show()
    {
        std::cout << "Widget::show" << std::endl;
    }
};
// 资源管理类的删除器
void widgetDeleter(Widget* w)
{
    std::cout << "widgetDeleter" << std::endl;
}

int priority()
{
    std::cout << "priority" << std::endl;
    throw std::exception();
    return 0;
}

void processWidget(std::shared_ptr<Widget> pw, int proprity)
{
    pw->show();
}

int main()
{
    try 
    {
        // 1.error
        // shared_ptr的ctor是explicit构造函数、无法进行隐式转换
        // processWidget(new Widget(), priority());  
        // 2.error
        // 如果执行顺序是这样,并且调用priority函数异常，就会又泄露
        // new
        // propriority
        // shared_ptr
        // mac下面没有触发
        //processWidget(std::shared_ptr<Widget>(new Widget, widgetDeleter), priority());
        // 3.ok
        std::shared_ptr<Widget> pw(new Widget, widgetDeleter);
        processWidget(pw, priority());
    }
    catch(std::exception& e)
    {
        std::cout << "catch exception" << std::endl;
    }


    return 0;
}