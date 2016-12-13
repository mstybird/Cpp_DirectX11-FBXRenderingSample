#include<cstdio>
#define 構造体(名前,...) struct 名前{__VA_ARGS__};
#define 符号付き整数変数(名前) int (名前);
#define 符号無し整数変数(名前) unsigned int 名前;
#define エントリーポイント int main()
#define スコープのはじめ {
#define スコープのおわり }

struct k {
	int (x);
};

構造体(ﾝｺﾞｺﾞ, 符号付き整数変数(えっくす))

エントリーポイント スコープのはじめ
ﾝｺﾞｺﾞ めう;
めう.えっくす = 10;
スコープのおわり