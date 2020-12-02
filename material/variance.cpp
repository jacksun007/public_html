#include <iostream>
#include <math.h>

template<typename T>
double to_double(T v) {
	/* if T cannot be casted to double compiler will complain */
	return (double)v;
}

template<>
double to_double(const char * v) {
	char * endptr;
	double dv = strtod(v, &endptr);
	if (endptr != nullptr && *endptr != '\0')
		throw std::runtime_error("cannot convert string to double");
	return dv;
}

double average(double n) {
	(void)n;
	return 0.;
}

template<typename T, typename... Args>
double average(double n, T v, Args... args) {
	double dv = to_double(v);
	return dv / n + average(n, args...);
}

double sum_of_difference_squared(double avg) {
	(void)avg;
	return 0.;
}

template<typename T, typename... Args>
double sum_of_difference_squared(double avg, T v, Args... args) {
	double dv = to_double(v);
	double diff = (dv - avg);
	return diff*diff + sum_of_difference_squared(avg, args...);
}

template<typename... Args>
double variance(Args... args) {
	double num = (double)sizeof...(Args); // number of arguments
	double avg = average(num, args...);
	double sum = sum_of_difference_squared(avg, args...);
	return sum / num;
}

class Complex {
	double r;
	double i;
public:
	Complex(double r, double i) : r(r), i(i) {}
	operator double() {
		return r;
	}
};

int main()
{
  double v = variance(702, "422", Complex(631, 4), 834, 892, 104, 772);
  printf("The variance of all values is: %lf\n", v);
  printf("The stddev of all values is: %lf\n", sqrt(v));
  return 0;
}