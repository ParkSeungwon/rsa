#include<iostream>
#include"rsa.h"
using namespace std;

int main()
{
	int p[] = {13, 11, 3, 5,7};
	int q[] = {31, 31, 17, 17, 17};
	int e[] = {19, 7, 5, 7, 29};
	int M[] = {2, 4, 5, 6, 3};

	for(int j=0; j<5; j++) {
		long K = p[j] * q[j];
		long phi = lcm(p[j]-1, q[j]-1);
		long i;
		for(i=1; (phi * i + 1) % e[j]; i++);
		long d = (phi * i + 1) / e[j];
		cout << d << ' ' << code(M[j], d, K) << endl;
	}
}
