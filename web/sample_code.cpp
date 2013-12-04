#include <iostream>
using namespace std;

template<class T>
void tprint(T t) {
	cout << t << endl;
}

class A {
public:
	virtual int bar() {
		bar();
		tprint(1);
		tprint(1.1);
		return 2;
	}
};

class B : public A{
public:
	virtual int bar() {
		bar();
		tprint('a');
		tprint(1);
		tprint("char");
		return 3;
	}
};

namespace N {

	void bar() 
	{
		tprint("hello");
	}

}

void bar()
{
	A *a;
	if (3 > 2) {
		a = new B();
	} else {
		a = new A();
	}
	a->bar();
}

int foo() {
	A a;
	a.bar();
	B *b = new B();
	b->bar();
	return 0;
}

void foo2(double, long) {
	A *a = new B();
	a->bar();
}

double foo(int a) {
	foo2(0.0, 1);
	return 3.14;
}

int main() {
	foo();
	foo(1);
	bar();
	N::bar();
	int a = 2;
	if (a > 0) {
		//cout << a << endl;
	} else {
	//	cout << a - 1 << endl;
	}
	return 0;
}
