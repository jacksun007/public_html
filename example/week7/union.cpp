#include <iostream>
#include <cassert>

using namespace std;

struct Tagged {
	enum { INT, STR } tag;
	union {			// anonymous union (members
		int * i;		// can enter parent scope)
		string * s;
	};
	Tagged(int i) : tag(INT), i(new int(i)) {}
	Tagged(const char * s) : tag(STR), s(new string(s)) {}
	~Tagged() {
		destroy();
	}
	
	void destroy() {
		if (tag == INT) delete i; else delete s;
	}

	Tagged(const Tagged & rhs) {
		tag = rhs.tag;
		if (rhs.tag == INT)
			i = new int(*rhs.i);
		else
			s = new string(*rhs.s);
	}
	
	bool operator==( const Tagged & rhs) {
		if (rhs.tag == INT)
			return *i == *rhs.i;
		else
			return *s == *rhs.s;
	}

	const string * get_str() const {
		if (tag == INT)
			return nullptr;
		else
			return s;
	}

    void set_str(const string & s) {
		destroy();
		this->s = new string(s);
		this->tag = STR;
	}
	
	friend ostream & operator<<(ostream & os, const Tagged & tu);
};

ostream & operator<<(ostream & os, const Tagged & tu)
{
	if (tu.tag == Tagged::INT)
		os << *tu.i << " (INT)";
	else
		os << *tu.s << " (STR)";
	return os;
}

int main() {
	Tagged t(5);
	cout << t << endl;
	assert(t.get_str() == nullptr);
	Tagged g = t;
	assert(g == t);
	t.set_str("hello world");
	cout << t << endl;
	assert(!(g == t));
	return 0;
}


