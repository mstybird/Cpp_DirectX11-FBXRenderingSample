#include<cstdio>
#include"CppTweener.h"
#include<time.h>
using namespace tween;
class test:public tween::TweenerListener {
public:

	void SetTweener(tween::Tweener* aTweener) {
		mTweener = aTweener;
	}

	void AddParam(TweenerParam& param) {
		mTweener->addTween(param);
	}

	void onStart(TweenerParam& param)override {
		std::cout << "Come tween\n";
	}

	void onStep(TweenerParam& param)override {
		std::cout << "Step:" << param.timeCount << "\n";
	}
	
	void onComplete(TweenerParam& param)override {
		std::cout << "Complete"<< "\n";
	}

	void Update() {
		mTweener->step(clock());
	}

	tween::Tweener* mTweener;

};



test t;
float a = 0;
float px = 10, py = 10; // for now just accept float properties
tween::Tweener gTweener;
void setup() {
	t.SetTweener(&gTweener);
	tween::TweenerParam param(500, tween::CUBIC, tween::EASE_OUT); //2 seconds tween

	param.addProperty(&px, 200);

	param.addProperty(&py, 30);
	t.AddParam(param);
	
}

void update() {
	t.Update();	
}

int main() {

	setup();

	//tween::TweenerParam param(500, tween::CUBIC, tween::EASE_OUT); //2 seconds tween

	//param.addProperty(&a, 100);
	//tweener.addTween(param);
	while (1)
	{
		printf("%2.2f:%2.2f:%2.2f\n", px, py,a);
		update();
	}
}
