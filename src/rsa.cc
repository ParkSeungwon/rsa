#include<vector>
#include<string>
#include<thread>
#include<condition_variable>
#include<mutex>
#include<atomic>
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
{//read unsigned int as characters
	u.n = n;
	string s;
	for(int i=0; i<3; i++) s += u.c[i];
	return s;
}

atomic<int> using_cpu {0};
mutex mtx;
condition_variable cv;
void decode(long m, long d, long K, vector<long>& v, int n)
{
	static int cpu = thread::hardware_concurrency();
	unique_lock<mutex> lck{mtx};
	while(using_cpu >= cpu) cv.wait(lck);
	if(using_cpu < cpu-1) {
		using_cpu++;
		lck.unlock();
		cv.notify_one();
		v[n] = code(m, d, K);
		using_cpu--;
	} else {
		using_cpu++;
		v[n] = code(m, d, K);
		using_cpu--;
		cv.notify_one();
	}
}

