#include<Config.hpp>

using namespace std;
using namespace Convention;

class A
{
public:
	virtual void a() {}
};

class B
{
public:
	virtual void b() {}
};

class C :public A, protected B
{
public:
	virtual void c() {}
};

int main()
{
	C c;
	((A)c).a();
}
