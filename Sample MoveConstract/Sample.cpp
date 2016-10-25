#include "Sample.h"

Sample::Sample():
	mX{ 1 },
	mY{ 2 },
	mZ{ 3 }
{
}

Sample::~Sample()
{
	int a = 0;
}

/*
Sample::Sample(const Sample & aObj)
{
	mX = aObj.mX;
	mY = aObj.mY;
	mZ = aObj.mZ;
}
*/


Sample::Sample(Sample && aObj) noexcept
{
	mX = aObj.mX;
	mY = aObj.mY;
	mZ = aObj.mZ;

	aObj.mX = 0;
	aObj.mY = 0;
	aObj.mZ = 0;
}
