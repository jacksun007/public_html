#include <iostream>

using namespace std;

// Type A with a serialize method.
struct A {
    string serialize() const {
        return "I am a A!";
    }
};

// Type B without a serialize method.
struct B
{
    int x;
};

string to_string(const B&)
{
    return "I am a B!";
}

// Type C with a "wrong" serialize member (not a method) and a to_string overload.
struct C
{
    string serialize;
};

string to_string(const C&)
{
    return "I am a C!";
}

struct D
{
    struct Functor {
        string operator()() {
            return "I am a D!";
        }
    };
    Functor serialize;
};

template<typename T>
string to_string(const T &)
{
    return "Default to_string";
}

struct Default {
    int foo() const {return 1;}
};

struct NoDefault {
    NoDefault(const NoDefault&) {}
    int foo() const {return 1;}
};

#define FAKEREF(T) (*(T *)nullptr)

template <class T> struct has_serialize
{
    // Test if T has serialize using decltype and declval.
    template <typename C> 
	static constexpr decltype(std::declval<C>().serialize(), bool()) test(int) {
        return true;
    }

    template <typename C> static constexpr bool test(...) {
        return false;
    }

    static constexpr bool value = test<T>(0);
};

/*
 * NOTE: enable_if is part of std library in C++11, so we don't re-define it
 */
 
/*
template <class T> 
typename enable_if<has_serialize<T>::value, string>::type 
make_packet(const T& obj) {
    return obj.serialize();
}

template <class T> 
typename enable_if<!has_serialize<T>::value, string>::type 
make_packet(const T& obj) {
    return to_string(obj);
}
*/

/*
 * possible implementation of std::true_type and false_type
 */

struct vrai_type {
	enum { value = true };
};

struct faux_type {
	enum { value = false };
};

template<typename T, typename = std::string>
struct can_serialize : faux_type {};

template <typename T>
struct can_serialize<T, decltype(std::declval<T>().serialize())>
	: vrai_type {};



template<typename, typename=void> 
struct is_incrementable : std::false_type { }; 

template<typename T> 
struct is_incrementable<T, 
	std::void_t<decltype(++std::declval<T&>())> 
> : std::true_type { };

template<typename, typename, typename=void> 
struct can_assign : std::false_type { }; 

template<typename T, typename U> 
struct can_assign<T, U,
	std::void_t<decltype(std::declval<T&>() = std::declval<U&>())> 
> : std::true_type { };

int main()
{
    A a;
    decltype(a.serialize()) test = "test";
	decltype(A().serialize()) t2 = "test 2";
	decltype(FAKEREF(NoDefault).foo()) ndf = 5;
	decltype(std::declval<NoDefault &>().foo()) nd2 = ndf;
	
	cout << test << ndf << nd2 << endl;
	
	cout << "----------------" << endl;
	
	cout << has_serialize<A>::value << endl;
    cout << has_serialize<B>::value << endl;
    cout << has_serialize<C>::value << endl;
	cout << has_serialize<D>::value << endl;
	
	cout << "----------------" << endl;
	
	cout << can_serialize<A>::value << endl;
    cout << can_serialize<B>::value << endl;
    cout << can_serialize<C>::value << endl;
	cout << can_serialize<D>::value << endl;
	
	cout << "----------------" << endl;
	
	cout << is_incrementable<int>::value << endl;
	cout << is_incrementable<string>::value << endl;
	cout << is_incrementable<A>::value << endl;
	
	cout << "----------------" << endl;
	
	cout << can_assign<int, double>::value << '\n';  // 1
	cout << can_assign<int, string>::value << '\n';  // 0
	cout << can_assign<double, long>::value << '\n'; // 1
	
	int i;
	double d = 5.312;
	
	// yup, can assign
	i = d;
	d = 123L;
	
	(void)i;
    return 0;
}
