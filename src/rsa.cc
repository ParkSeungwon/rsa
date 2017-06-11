#include<vector>
#include<string>
#include<iostream>
using namespace std;

int lcm(int a, int b) 
{//least common multiple
	for(int i=1; ; i++) if(a*i % b == 0) return a*i;
}
		
long code(long m, long e, long K) 
{//m^e%K
	long sum = 1;
	for(long i=0; i<e; i++) {
		sum *= m;
		sum %= K;
	}
	return sum;
}

static union {
	unsigned int n;
	unsigned char c[4];
} u;

vector<unsigned int> stovi(string s)//read characters as uint
{//just use 3 character to be inside of long limit when go exponetial
	vector<unsigned int> v;
	while(s.size() % 3) s += ' ';
	for(int i=0; i<s.size(); i+=3) {
		for(int j=0; j<3; j++) u.c[j] = s[i+j];
		u.c[4] = 0;
		v.push_back(u.n);
	}
	return v;
}

string vitos(unsigned int n)
{//read unsigned int as characters
	u.n = n;
	string s;
	for(int i=0; i<3; i++) s += u.c[i];
	return s;
}

vector<int> primitive_root(int n)//n should be prime number
{//find primitive root for diffie hellman algorithm
	vector<int> v;
	bool ar[n+1];// 1 ~ j-1 slot to hit
	ar[n] = false;// stop sign
	for(int i=2,j; i<n; i++) {//test if 'i' is primitive
		for(j=0; j<n; j++) ar[j] = false;//clear
		for(j=i; !ar[j]; j=i*j%n) ar[j] = true;//mark hit until repeat
		for(j=1; ar[j]; j++);//advance until no hit
		if(j == n) v.push_back(i);
	}
	return v;
}






