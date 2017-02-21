#include<iostream>
#include<functional>
#include"core.h"
using namespace std;

int f(int n) {
	return 5;
}

int main()
{
	AutoThread at;
	auto a = at.add_thread(bind(f, 3));
	cout << a.get() << endl;
	decltype(declval(f())) a;
}

