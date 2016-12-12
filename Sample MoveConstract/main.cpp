#include"Sample.h"
#include<Windows.h>
#include<iostream>
#include<ctime>
#include<vector>
#include<thread>
#include<chrono>
#include<sstream>
#include<string>
#include<algorithm>
using namespace std;
using namespace std::chrono;
int main() {
	
	string str = "FPS\nVP Under\nGIve Way";
	stringstream ss;
	string s1, s2, s3;
	ss << "FPS\n";
	ss << "VP Under\n";
	ss << "Give Way";
	s1 = ss.str();
	replace(s1.begin(), s1.end(), ' ', (char)-127);
	stringstream ss2(s1);
	ss2 >> s1;
	ss2 >> s2;
	ss2 >> s3;
	replace(s2.begin(), s2.end(), (char)-127, ' ');
	replace(s3.begin(), s3.end(), (char)-127, ' ');

	system_clock::time_point start;
	system_clock::time_point end;

	auto lstart = system_clock::now();
	Sleep(1001);
	auto lend = system_clock::now();
	auto v=std::chrono::duration_cast<milliseconds>(lend-lstart);
	std::cout << v.count() << std::endl;
}