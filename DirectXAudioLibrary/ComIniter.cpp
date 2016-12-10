#include "ComIniter.h"
#include<Windows.h>
#include<cstdio>
ComIniter::ComIniter()
{
	if (CoInitializeEx(nullptr, COINIT_MULTITHREADED)) {
		printf("Failed To Initialize COM\n");
	}
}

ComIniter::~ComIniter()
{
	CoUninitialize();
}
