#include<vector>
using namespace std;

vector<int> primitive( int n) // n should be prime number
{// find primitive root for diffie hellman algorithm
	vector<int> v;
	bool ar[n +1];// 1  ̃ j−1 slot to hit
	ar[n] = false ;// stop sign
	for( int i =2, j ; i<n; i++) {// test if ' i ' is primitive
		for( j =0; j<n; j++) ar[ j ] = false ;// clear
		for( j=i ; ! ar[ j ]; j=i*j%n) ar[ j ] = true ;// mark hit until repeat
		for( j =1; ar[ j ]; j++) ;// advance until no hit
		if ( j == n) v.push_back(i ) ;
	}
	return v;
}

