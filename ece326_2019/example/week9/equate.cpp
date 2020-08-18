#include <iostream>

using namespace std;

template<class T, class U>
struct can_equate 
{	
	// case 1: can we do X == Y
	template<class X, class Y>
	static constexpr decltype(std::declval<X&>() == std::declval<Y&>(), bool())
	test(long) {
		return true;
	}
	
	// case 2: can we do Y == X
	template<class X, class Y>
	static constexpr decltype(std::declval<Y&>() == std::declval<X&>(), bool())
	test(int) {
		return true;
	}
	
	template<class X, class Y>
	static constexpr bool test(...) { return false; }
	
	static constexpr bool value = test<T, U>(0);
};

/*
 * Note: if you use the true_type/false_type method, then you can only specify
 *       one expression. If you use X == Y or Y == X and one of them fails, the
 *       entire expression fails and that's not the desirable behaviour.
 */

struct Complex {
	int r;
	int i;
	
	/* only Complex == int works, int == Complex does not */
	bool operator==(int rhs) {
		if (i == 0) {
			return r == rhs;
		}
		
		return false;
	}
};

int main()
{
	cout << can_equate<int, double>::value << endl;    
	cout << can_equate<double, int>::value << endl;    
	
	cout << can_equate<int, string>::value << endl;    
	cout << can_equate<string, int>::value << endl;    
	
	cout << can_equate<int, long>::value << endl;    
	cout << can_equate<long, int>::value << endl;    

	cout << can_equate<int, Complex>::value << endl;  
	cout << can_equate<Complex, int>::value << endl;  
	
	return 0;
};