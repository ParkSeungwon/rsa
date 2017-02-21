#include<iostream>
#include<random>
#include<algorithm>
#include<thread>
#include"rsa.h"
#include"core.h"
#define M 5000
using namespace std;

int main()
{//generate rsa key and encode sample
	const Prime<5000> prime;//contains 5000 prime numbers
	auto it = lower_bound(prime.arr, prime.arr+M, 10000);//select only over 10000
	uniform_int_distribution<> di(it - prime.arr, M-1);
	random_device rd;

	//resolve p, q, K, e, d, phi
	int p = prime.arr[di(rd)];
	int q = prime.arr[di(rd)];
	long K = p * q;
	long phi = lcm(p-1, q-1);
	long i;
	for(i=di(rd); phi % prime.arr[i] == 0; i++);
	int e = prime.arr[i];
	for(i=1; (phi * i + 1) % e; i++);
	long d = (phi * i + 1) / e;
	cout << "p = " << p << ", q = " << q << ", phi = " << phi << endl;
	cout << "K = " << K << ", e = " << e << " , d = " << d << endl;

	//encode
	cout << "enter message to encode" << endl;
	string s;
	getline(cin, s);
	vector<long> v;
	for(auto& a : stovi(s)) v.push_back(code(a, e, K));
	for(auto& a : v) cout << a << ' '; cout << endl;

	//decode
	AutoThread at;
	vector<future<Any>> vf;//cause code return long
	for(int i=0; i<v.size(); i++) vf.push_back(at.add_thread(bind(code, v[i], d, K)));
	for(int i=0; i<v.size(); i++) cout << vitos((long)vf[i].get()); cout << endl;
}



