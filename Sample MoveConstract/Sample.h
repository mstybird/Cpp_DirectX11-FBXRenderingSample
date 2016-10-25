#pragma once

class Sample {
public:
	Sample();
	~Sample();
	Sample(Sample& aObj) {
		aObj.mX = 0;
	}
	Sample(Sample&& aObj) noexcept;
	int* Get() {
		return p;
	}
//private:
public:
	int *p;
	int mX;
	int mY;
	int mZ;
};
