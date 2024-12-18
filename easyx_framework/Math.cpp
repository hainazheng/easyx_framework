#include "Math.h"
#include<random>


///获取整数类型随机数
int Math::RandInt(int lower, int upper)
{
    if (lower > upper)
    {
        int tmp = upper;
        upper =lower ;
        lower = tmp;
    }
    std::random_device rd;      //用熵来实现真正的随机，也就是计算机的混乱状态
    std::mt19937 gen(rd());         //运用梅森算法生成随机数种子设置为rd，实现真随机
    std::uniform_int_distribution<int> dis(lower, upper);       //生成一个平均分布的范围，实现宏观的分布

    return dis(gen);            //这里gen不能加括号，加了就是生成下一次

}

///获取实数类型随机数
double Math::RandReal(double lower, double upper)
{
    if (lower > upper)
    {
        double tmp = upper;
        upper = lower;
        lower = tmp;
    }
    std::random_device rd;      //用熵来实现真正的随机，也就是计算机的混乱状态
    std::mt19937 gen(rd());         //运用梅森算法生成随机数种子设置为rd，实现真随机
    std::uniform_real_distribution<double> dis(lower, upper);       //生成一个平均分布的范围，实现宏观的分布

    return dis(gen);            //这里gen不能加括号，加了就是生成下一次
}

///随机生成[0,1)的随机数，用来生成比例
double Math::RandPerc()
{
    std::random_device rd;      //用熵来实现真正的随机，也就是计算机的混乱状态
    std::mt19937 gen(rd());         //运用梅森算法生成随机数种子设置为rd，实现真随机
    return std::generate_canonical<double,10>(gen);
}

double Math::Clamp(double value, double lower, double upper)
{
    return min(max(value,min(lower,upper)),max(lower,upper));
}

double Math::mapping(double value, double oldlower, double oldupper, double newlower, double newupper)
{
    double newvalue= (max(newlower, newupper) - min(newlower, newupper))*((value-min(oldlower, oldupper)) /(max(oldlower, oldupper) - min(oldlower, oldupper)))+ min(newlower, newupper);
    return newvalue;
}
