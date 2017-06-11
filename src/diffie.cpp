#include<iostream>
#include<random>
#include<algorithm>
#include"rsa.h"
#define M 5000
using namespace std;

int main(int ac, char** av)
{
	const Prime<5000> prime;//contains 5000 prime numbers
	auto it = lower_bound(prime.arr, prime.arr+M, 10000);//select only over 10000
	uniform_int_distribution<> di(it - prime.arr, M-1);
	random_device rd;

	int q = prime.arr[di(rd)];
	auto v = primitive_root(q);
	auto it2 = lower_bound(v.begin(), v.end(), 5000);
	uniform_int_distribution<> di2(it2 - v.begin(), v.size()-1);
	int a = v[di2(rd)];
	int xa = prime.arr[di(rd)];//secret key
	int xb = prime.arr[di(rd)];//secret key
	int ya=1, yb=1, K=1;//ya, yb public key, K = final key
	for(int i=0; i<xa; i++) ya = ya * a % q;//ya = a^xa % q
	for(int i=0; i<xb; i++) yb = yb * a % q;//yb = a^xb % q
	for(int i=0; i<xa*xb; i++) K = K * a % q;//K = ya^xb % q = yb^xa % q = a^(xa*xb)%q
	cout << "q : " << q << ", a : " << a << endl;
	cout << "xa : " << xa << ", xb : " << xb << endl;
	cout << "ya : " << ya << ", yb : " << yb << endl;
	cout << "K : " << K << endl;
}
