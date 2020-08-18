#include <iostream>

using namespace std;

/*
 * C++98 style to check for existence of method
 */
template <class T> struct has_serialize
{
    typedef char yes[1]; typedef char no[2]; static char tm[2];

    /* checks if class T really has serialize method (not field) */
    template<typename U, U u> struct really;
    
    /* class T has serialize */
    template<typename Z> static yes& test(really<string(Z::*)(), &Z::serialize>*) { return tm; }
    template<typename Z> static yes& test(really<string(Z::*)() const, &Z::serialize>*) { return tm; }

    /* SFINAE - class T does not have serialize */
    template<typename> static no& test(...) { return tm; }

    // The constant used as a return value for the test.
    static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

// Type A with a serialize method.
struct A {
    string serialize() const {
        return "I am a A!";
    }
};

// afunc_t is a type alias to 'string (A::*)() const'
typedef string (A::* afunc_t)() const;

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

/*
 * NOTE: enable_if is part of std library in C++11, so we don't re-define it
 */
 
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

int main()
{
    A a;
    afunc_t af = &A::serialize;
    cout << (a.*af)() << endl;
    
    A * ap = &a;
    cout << (ap->*af)() << endl;

    cout << has_serialize<A>::value << endl;
    cout << has_serialize<B>::value << endl;
    cout << has_serialize<C>::value << endl;
    
    cout << "----------------" << endl;
    
    D d;
    cout << d.serialize() << endl;      
    cout << has_serialize<D>::value << endl; 

    cout << "----------------" << endl;

    B b; C c;
    
    cout << make_packet(a) << endl; 
    cout << make_packet(b) << endl; 
    cout << make_packet(c) << endl; 
    cout << make_packet(d) << endl; 

    return 0;
}
