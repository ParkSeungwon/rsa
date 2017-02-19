#include<vector>
#include<string>
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

vector<unsigned int> stovi(string s)
{
	vector<unsigned int> v;
	const char* p = s.data();
	while(s.size() % 3) s += ' ';
	for(int i=0; i<s.size(); i+=3) {
		for(int j=0; j<3; j++) u.c[j] = *(p+i+j); 
		u.c[4] = 0;
		v.push_back(u.n);
	}
	return v;
}

string vitos(unsigned int n)
{
	u.n = n;
	string s;
	for(int i=0; i<3; i++) s += u.c[i];
	return s;
}
