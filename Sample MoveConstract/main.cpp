#include"Sample.h"
#include<iostream>
#include<algorithm>
#include<vector>
using std::vector;
using std::move;
using std::cout;
using std::endl;

template<typename T>
void Print(const T& aData) {
	cout << "Print " << typeid(aData).name() << endl;
	auto lCount = 0;
	for (decltype(auto)lInstance : aData) {
		cout<<lCount<<" :" << lInstance << endl;
		++lCount;
	}
	cout << "Print End" << endl;
}

/*
Sample Create(...) {

}
*/

void f(int*&& p) {

}

int main() {
	vector<int>lData{ 1,2,3,4,5,6 };
	vector<int>lData2{};

	lData2 = move(lData);

	Print(lData);

	Print(lData2);
	
	Sample s;
	//f(std::move(s.Get()));
	Sample s1 = std::move(s);
	//Sample s1 = s;

	s.Get();
}