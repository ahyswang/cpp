#include <iostream>


struct Day {
    // 防止隐式转换
    explicit Day(int d)
        : val(d) {}
    int val;
};

struct Month {
    explicit Month(int m)
        : val(m) {}
    int val;
};

struct Year {
    explicit Year(int y)
        : val(y) {}
    int val;
};

class Date {
public:
    // 强制传递指定类型的对象
    Date(const Month& m, const Day& d, const Year& y)
        : month(m.val)
        , day(d.val)
        , year(y.val)
    {

    }
private:
    Year year;
    Month month;
    Day day;
};

/*
int showDay(Day d)
{
    std::cout << d.val << std::endl;
    return 0;
}
*/

int main()
{
    //error
    //explicit 阻止隐式转换
    //showDay(10);
    //error
    //自定义了默认构造函数，编译不会自动加上无参数默认构造函数
    //Day d;
    
    //ok
    Date(Month(10), Day(28), Year(2010));

    return 0;
}