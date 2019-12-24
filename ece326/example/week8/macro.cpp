#include <iostream>
#include <vector>

using namespace std;

/* fixed version */
#define PP_NARG(...) PP_NARG_(~, ##__VA_ARGS__, PP_RSEQ_N()) 
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__) 
#define PP_ARG_N( _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N 
#define PP_RSEQ_N() 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define DEFINE_STACK(CLS, TYPE) class CLS { \
	std::vector<TYPE *> array; \
public: \
	CLS() {} \
	~CLS() { \
		for (auto it = array.begin(); it != array.end(); it++) \
			delete *it; \
	} \
	unsigned push(TYPE * v) { \
		unsigned idx = array.size(); \
		array.push_back(v); \
		return idx; \
	} \
	TYPE * pop() { \
		if (array.size() == 0) return nullptr; \
		TYPE * ret = array.back(); \
		array.pop_back(); \
		return ret; \
	} \
	template<typename... Args> \
	void emplace(Args... args) { \
		array.push_back(new TYPE(std::forward<Args>(args)...)); \
	} \
	void print() const { \
		for (auto it = array.begin(); it != array.end(); it++) \
			cout << *(*it) << " "; \
		cout << "\n"; \
	} \
}

class Complex {
	double r;
	double i;
public:
	Complex(double r, double i) : r(r), i(i) {}
	friend ostream & operator<<(ostream &, const Complex &);
};

ostream & operator<<(ostream & os, const Complex & c)
{
	os << c.r;
	if (c.i > 0) {
		os << " + " << c.i << "i";
	}
	else if (c.i < 0) {
		os << " - " << -c.i << "i";
	}
	return os;
}

DEFINE_STACK(IntStack, int);
DEFINE_STACK(ComplexStack, Complex);

int main() {
	cout << "number of arguments: " << PP_NARG() << endl;
	cout << "number of arguments: " << PP_NARG(1, 2, 3) << endl;
	
	IntStack is;
	is.emplace(3);
	is.emplace(6);
	is.emplace(2);
	is.emplace(12);
	is.emplace(53);
	int * top = is.pop();
	cout << "top: " << *top << endl;
	delete top;
	is.print();
	
	ComplexStack cs;
	Complex * c = new Complex(1, 2);
	cs.push(c);
	cs.emplace(4, 6);
	cs.print();
	
	return 0;
}
