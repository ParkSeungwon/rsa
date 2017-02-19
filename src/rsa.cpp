#include<iostream>
#include<random>
#include<algorithm>
#include<thread>
#include"rsa.h"
#define M 5000
using namespace std;

int main()
{//generate rsa key and encode sample
	Prime<5000> prime;
	auto it = lower_bound(prime.arr, prime.arr+M, 10000);
	uniform_int_distribution<> di(it - prime.arr, M-1);
	random_device rd;

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
	cout << "enter message to encode" << endl;
	string s;
	cin >> s;//reuse
	vector<long> v;
	for(auto& a : stovi(s)) cout << a << ' ';
	cout << endl;
	for(auto& a : stovi(s)) v.push_back(code(a, e, K));
	for(auto& a : v) cout << "encoded to " << a << ' ';
	cout << endl << "decoded to ";
	vector<long> vl;
	vl.resize(v.size());
	thread threads[v.size()];
	for(int i=0; i<v.size(); i++) 
		threads[i] = thread(decode, v[i], d, K, ref(vl), i);
	for(int i=0; i<v.size(); i++) threads[i].join();
	for(auto& a : vl) cout << vitos(a); 
//	for(auto& a : v) cout << vitos((code(a, d, K)));
}



