#include "library.h"

#include <iostream>
using namespace std;

/*
void lib2() {
	cout << "LIB1" << endl;
}
*/

// Why this function will cause two CFG?
//void lib1() {
//	cout << "OK" << endl;
//}

int test1(int i) {
	cout << "E" << endl;
	if (i > 0)
		return 1;
	else
		return 2;
	cout << "ok" << endl;
}
