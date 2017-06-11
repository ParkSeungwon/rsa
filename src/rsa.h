#pragma once
#include<vector>
#include<string>

int lcm(int a, int b);
long code(long m, long e, long K);
std::vector<unsigned int> stovi(std::string s);
std::string vitos(unsigned int n);
std::vector<int> primitive_root(int n);

template<int N> class Prime
{//generate N Prime numbers in compile time
public:
	unsigned int arr[N]{};
	constexpr Prime();
};

