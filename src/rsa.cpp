#include<iostream>
#include<random>
#include<algorithm>
#include<thread>
#include"rsa.h"
#define M 5000
using namespace std;

vector<int> primitive(int);
int main(int ac, char** av)
{//generate rsa key and encode sample
	const Prime<5000> prime;
	auto it = lower_bound(prime.arr, prime.arr+M, 10000);
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
	for(auto& a : v) cout << a << ' ';

	//decode
	vector<long> vl;
	vl.resize(p = v.size());//reuse
	thread threads[p];
	for(int i=0; i<p; i++) threads[i] = thread(decode, v[i], d, K, ref(vl), i);
	for(int i=0; i<p; i++) threads[i].join();
	for(auto& a : vl) cout << vitos(a); 

	cout << endl;
	for(auto i : primitive(atoi(av[1]))) cout << i << ',';
	cout << endl;
}



