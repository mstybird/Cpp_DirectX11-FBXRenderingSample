#include"Sample.h"
#include<Windows.h>
#include<iostream>
#include<ctime>
#include<vector>
#include<thread>
#include<chrono>
using namespace std;
using namespace std::chrono;
int main() {
	system_clock::time_point start;
	system_clock::time_point end;

	auto lstart = system_clock::now();
	Sleep(1001);
	auto lend = system_clock::now();
	auto v=std::chrono::duration_cast<milliseconds>(lend-lstart);
	std::cout << v.count() << std::endl;
}