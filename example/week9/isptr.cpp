#include <iostream>

using namespace std;

// base template
template<class T> struct rm_const_helper { typedef T type; };
// partial specialization
template<class T> struct rm_const_helper<const T> { typedef T type; };
template<class T> struct rm_const : rm_const_helper<T> {};

// base template
template<class T> struct is_ptr_helper : std::false_type {};
// partial specialization
template<class T> struct is_ptr_helper<T *> : std::true_type {};
// uses helper and rm_const to remove const from pointer
template<class T> struct is_ptr : is_ptr_helper<typename rm_const<T>::type> {};

int main()
{
	cout << std::is_const<rm_const<const int>::type>::value << endl;    
	cout << std::is_const<rm_const<int>::type>::value << endl; 
	cout << std::is_const<rm_const<int * const>::type>::value << endl; 
	cout << std::is_const<rm_const<const int *>::type>::value << endl;
	
    cout << is_ptr<int>::value << endl;   		 // 0
	cout << is_ptr<int *>::value << endl;  		 // 1
	cout << is_ptr<int **>::value << endl; 		 // 1
	cout << is_ptr<int &>::value << endl;  		 // 0
	cout << is_ptr<int * const>::value << endl;  // 1
	
	return 0;
};