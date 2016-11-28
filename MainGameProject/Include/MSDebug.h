#pragma once

#ifdef _DEBUG
#include<cstdio>
#include<cassert>

#define MSLOG(format,...)\
printf(format,__VA_ARGS__)


#endif