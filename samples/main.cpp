#include <iostream>
using namespace std;

template<class T>
void tprint(T t) {
	cout << t << endl;
}

class A {
public:
	int bar() {
		bar();
		tprint(1);
		return 2;
	}
};

namespace B {

	void bar() 
	{
	}

}

void bar()
{
}

int foo() {
	A a;
	a.bar();
	return 0;
}

void foo2(double, long) {
}

double foo(int a) {
	foo2(0.0, 1);
	return 3.14;
}

int main() {
	foo();
	foo(1);
	bar();
	B::bar();
	int a = 2;
	if (a > 0) {
		//cout << a << endl;
	} else {
	//	cout << a - 1 << endl;
	}
	return 0;
}
