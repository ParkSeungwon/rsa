#include<iostream>
#include<functional>
#include"core.h"
using namespace std;

int f(int n) {
	return 5;
}

int main()
{
	Any a = "abdc";
	cout << (const char*)a << endl;
}

