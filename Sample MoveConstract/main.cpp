#include"Sample.h"
#include<iostream>
#include<ctime>
#include<vector>
#include<thread>
void sleepCX(const int ms) {
	clock_t start, end;
	start = clock();
	do {
		end = clock();
	} while (end - start < ms);
}

using namespace std;

void func(const int ms) {
	cout << "Start:" << ms << endl;
	cout << "EndFFF" << endl;
	sleepCX(ms);
	cout << "End" << endl;
}

int main() {

	vector<thread> th;
	
	thread t(func, 1000);
	//sleepCX(1000);
	t.join();
	sleepCX(100);
	th.push_back(move(t));
	sleepCX(100);
	th.clear();
}