#include <iostream>
#include <utility>

using namespace std;

template<typename T>  /* find index of val in array of size n */
int binary_search(const T & val, T * array, int n) {
	int top = n-1;
	int bot = 0;
	
	while (bot <= top) {
		int mid = (top + bot)/2;
		
		if (array[mid] == val)
			return mid;
		else if (array[mid] < val)
			bot = mid+1;
		else 
			top = mid-1;
	}

	return -1;
}

template<typename T>
struct Triple {
	T a, b, c;
	
	Triple() : a(0), b(0), c(0) {}
	
	Triple(T && a, T && b, T && c) 
	  : a(std::move(a))
	  , b(std::move(b)) 
	  , c(std::move(c))
	{}
	
	bool operator==(const Triple<T> & rhs) {
		return a == rhs.a && b == rhs.b && c == rhs.c;
	}
	
	bool operator<(const Triple<T> & rhs) {
		if ( a < rhs.a )
			return true;
		else if ( a > rhs.a )
			return false;
		else if ( b < rhs.b )
			return true;
		else if ( b > rhs.b )
			return false;
		else if ( c < rhs.c )
			return true;
		/* c >= rhs.c */
		return false;
	}	
};

int main() {
	Triple<int> array[] = { 
		Triple<int>(1, 0, 6),
		Triple<int>(3, 5, 9),
		Triple<int>(4, 5, 1),
		Triple<int>(6, 3, 0),
		Triple<int>(6, 3, 4),
		Triple<int>(6, 7, 8),
		Triple<int>(7, 8, 1),
		Triple<int>(8, 5, 5),
		Triple<int>(9, 0, 1),
		Triple<int>(9, 5, 0),
	};
	
	int size = sizeof(array)/sizeof(Triple<int>);
	Triple<int> x(9, 5, 0);

	cout << binary_search(x, array, size) << endl;
}
