#include<iostream>
using namespace std;
class A
{
public: void Func() {};    //ֻ��������ʵ��
};
class A1 : public A {
public:	 void Func()//�̳к�ʵ��	
{
	cout << "A1" << endl;
}
};
class A2 : public A {
public:	 void Func()        //�̳к�ʵ��	
{
	cout << "A2" << endl;
}
};

int main()
{
	A1 a1;
	A2 a2;
	A1 *aa ;
	aa = &a1;
	aa->Func();
	aa = (A1*)a2;
	system("pause");
}