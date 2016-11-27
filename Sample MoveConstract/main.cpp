#include"Sample.h"
#include<iostream>
#include<algorithm>
#include<vector>
#include<thread>
#include<Windows.h>
#include<future>

using namespace std;

void func(int start,int end) {
	for (int i = start; i < end; ++i) {
		cout << i;
	}
	cout << endl;
}

int main() {

	clock_t start, end;
	start = clock();

	std::packaged_task<void(int,int)> task1(
		[](int x, int y) {
		func(x,y);
	}
	);
	std::future<void> f1 = task1.get_future();

	std::packaged_task<void(int, int)> task2(
		[](int x, int y) {
		func(x, y);
	}
	);
	std::future<void> f2 = task2.get_future();

	task1(0, 5);
	task2(0, 5);
	end = clock();
	cout << end - start << endl;

	std::thread t;
	

}