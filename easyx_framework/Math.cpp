#include "Math.h"
#include<random>


///��ȡ�������������
int Math::RandInt(int lower, int upper)
{
    if (lower > upper)
    {
        int tmp = upper;
        upper =lower ;
        lower = tmp;
    }
    std::random_device rd;      //������ʵ�������������Ҳ���Ǽ�����Ļ���״̬
    std::mt19937 gen(rd());         //����÷ɭ�㷨�����������������Ϊrd��ʵ�������
    std::uniform_int_distribution<int> dis(lower, upper);       //����һ��ƽ���ֲ��ķ�Χ��ʵ�ֺ�۵ķֲ�

    return dis(gen);            //����gen���ܼ����ţ����˾���������һ��

}

///��ȡʵ�����������
double Math::RandReal(double lower, double upper)
{
    if (lower > upper)
    {
        double tmp = upper;
        upper = lower;
        lower = tmp;
    }
    std::random_device rd;      //������ʵ�������������Ҳ���Ǽ�����Ļ���״̬
    std::mt19937 gen(rd());         //����÷ɭ�㷨�����������������Ϊrd��ʵ�������
    std::uniform_real_distribution<double> dis(lower, upper);       //����һ��ƽ���ֲ��ķ�Χ��ʵ�ֺ�۵ķֲ�

    return dis(gen);            //����gen���ܼ����ţ����˾���������һ��
}

///�������[0,1)����������������ɱ���
double Math::RandPerc()
{
    std::random_device rd;      //������ʵ�������������Ҳ���Ǽ�����Ļ���״̬
    std::mt19937 gen(rd());         //����÷ɭ�㷨�����������������Ϊrd��ʵ�������
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
