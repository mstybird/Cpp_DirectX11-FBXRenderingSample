#include"MSProgressData.hxx"
#include<stdio.h>
MSProgress data;

void print() {
	printf("Max   :%3.2f\n",data.GetMax());
	printf("Now   :%3.2f\n", data.GetNow());
	printf("Min   :%3.2f\n", data.GetMin());
	printf("Remain:%3.2f\n",data.GetMaxRemainFix());
}
void printPer() {
	printf("Now   :%3.2f%%\n", data.GetNowPer()*100);
	printf("Remain:%3.2f%%\n",data.GetMaxRemainPer()*100);
}

int main() {
	data.Set(5, -5, 0);
	printPer();
	data.AddFix(1);
	printPer();
	return 0;
}