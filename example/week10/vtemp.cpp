#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

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
	
	Complex(int r, int i) : r(r), i(i) {}
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

template<typename T, typename... Args>
T make_and_print(Args&& ... args) {
	T obj(std::forward<Args>(args)...);
	cout << obj << endl;
	return obj;
}

template <template <typename, typename...> class ContainerType,
          typename ValueType, typename... Args>
void print_container(const ContainerType<ValueType, Args...>& c) {
  for (const auto& v : c) {
    std::cout << v << ' ';
  }
  std::cout << '\n';
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const std::pair<T, U>& p) {
  out << "(" << p.first << ", " << p.second << ")";
  return out;
}

template <class... Ts> struct Tuple {};

template <class T, class... Ts>
struct Tuple<T, Ts...> : Tuple<Ts...> {
  Tuple(T t, Ts... ts) : Tuple<Ts...>(ts...), data(t) {}

  T data;
};

template <size_t, class> struct elem_type_holder;

template <class T, class... Ts>
struct elem_type_holder<0, Tuple<T, Ts...>> {
  typedef T type;
};

template <size_t k, class T, class... Ts>
struct elem_type_holder<k, Tuple<T, Ts...>> {
  typedef typename elem_type_holder<k - 1, Tuple<Ts...>>::type type;
};

template <size_t k, class... Ts>
typename std::enable_if<k == 0, typename elem_type_holder<0, Tuple<Ts...>>::type&>::type
access(Tuple<Ts...>& t) {
    return t.data;
}

template <size_t k, class T, class... Ts>
typename std::enable_if<k != 0, typename elem_type_holder<k, Tuple<T, Ts...>>::type&>::type
access(Tuple<T, Ts...>& t) {
    Tuple<Ts...>& base = t;
    return access<k - 1>(base);
}

int main()
{
	auto c = make_and_print<Complex>(5, 7);
	c.i = -2;
	cout << c << endl;
	
	cout << "----------------" << endl;
	
	std::vector<double> vd{3.14, 8.1, 3.2, 1.0};
	print_container(vd);

	std::list<int> li{1, 2, 3, 5};
	print_container(li);
	
	std::unordered_map<string, int> msi{{"foo", 42}, {"bar", 81}, {"baz", 4}};
	print_container(msi);
	
	cout << "----------------" << endl;
	
	Tuple<double, int, const char*> t1(3.14, 42, "hello");
	cout << access<0>(t1) << endl;
	cout << access<1>(t1) << endl;
	cout << access<2>(t1) << endl;
	
	// can modify too, since access returns a mutable reference
	access<2>(t1) = "world";
	cout << access<2>(t1) << endl;
	
	//error: invalid use of incomplete type ‘struct elem_type_holder<0, Tuple<> >’
	//cout << access<3>(t1) << endl;
		
	return 0;
}