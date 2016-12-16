#include"MSThread.hxx"

void MSThread::Wait()
{
	if (mThread.joinable()) {
		mThread.join();
	}
}

MSThread::MSThread()
{
	mRegistFunction = []() {};
}

MSThread::~MSThread()
{
	Wait();
}

void MSThread::SetFunction(std::function<void()> aFunc)
{
	mRegistFunction = aFunc;
}

void MSThread::Start()
{
	mThread = std::move(std::thread(mRegistFunction));
}
