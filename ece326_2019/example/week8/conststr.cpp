#include <iostream>
#include <vector>

using namespace std;

class ConstStr {
	const char * str;
	unsigned len;
	
	constexpr bool endswith(const char * ss, int i, int n) const {
		return (n < 0) ? true : (
			(i < 0) ? false : str[i] == ss[n] && endswith(ss, i-1, n-1)
		);
	}
	
public:
	template<int N>
	constexpr ConstStr(const char (&s)[N]) : str(s), len(N-1) {}
	
	constexpr unsigned long hash(unsigned i=0, unsigned long h=5381) const {
		return (i == len) ? h : hash(i+1, ((h << 5) + h) + str[i]);
	}
	
	constexpr bool startswith(const char * ss, unsigned i=0) const {
		return (*ss == '\0') ? true : (
			(*ss == str[i]) ? startswith(ss+1, i+1) : false
		);
	}
	
	template<int N>
	constexpr bool endswith(const char (&ss)[N]) const {
		return endswith(ss, len, N-1);
	}
};

template<bool B>
void print_bool(const char * header) {
	cout << header << ((B) ? "yes" : "no") << endl;
}

int main() {
	constexpr ConstStr hello("hello world");
	
	print_bool<hello.startswith("hell")>("starts with hell? ");
	print_bool<hello.startswith("helicopter")>("starts with helicopter? ");
	print_bool<hello.endswith("world")>("ends with world? ");
	print_bool<hello.endswith("would")>("ends with would? ");
	print_bool<hello.endswith("super hello world")>("ends with super hello world? ");
	cout << hello.hash() << endl;
	
	return 0;
}
