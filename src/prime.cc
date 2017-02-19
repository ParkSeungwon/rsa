#include"rsa.h"
#include<iostream>
using namespace std;

template<int N> constexpr Prime<N>::Prime()
{
	int k=0;
	for(unsigned int i=2; k<N ;i++) {
		bool isPrime = true;
		for(int j=0; j<k; j++) {
			if(arr[j] > i/2) break;
			if(i % arr[j] == 0) {
				isPrime = false;
				break;
			}
		}
		if(isPrime) arr[k++] = i;
	}
}

template class Prime<5000>;

