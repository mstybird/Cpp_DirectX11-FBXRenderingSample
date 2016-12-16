#pragma once
#include<functional>
#include<thread>
//void‚Ì‚Ý‘Î‰ž‚Æ‚·‚é
class MSThread {
public:
	MSThread();
	~MSThread();
	void SetFunction(std::function<void()> aFunc);
	void Start();
	void Wait();

	std::function<void()> mRegistFunction;
	std::thread mThread;
};
