#include"Sample.h"
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
class A {
public:
	A() {
		cout << "A Start" << endl;
	}
	~A() {
		cout << "A End" << endl;
	}
};

class B:public A {
public:
	B() {
		cout << "B Start" << endl;
	}
	~B() {
		cout << "B End" << endl;
	}
};


int main() {
	B* pA = new B;
	delete pA;

}