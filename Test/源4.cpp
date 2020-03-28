#include<iostream>
using namespace std;
class A
{
public: void Func() {};    //只声明，不实现
};
class A1 : public A {
public:	 void Func()//继承后实现	
{
	cout << "A1" << endl;
}
};
class A2 : public A {
public:	 void Func()        //继承后实现	
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