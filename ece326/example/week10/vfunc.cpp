#include <cstdarg>	// provides variable argument handling
#include <cstdio>

int eprintf(const char * fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int ret = vfprintf(stderr, fmt, args);
	va_end(args);
	return ret;
}

int find_max(int n, ...)
{
	int i, val, largest;
	va_list vl;
	va_start(vl, n);
	largest = va_arg(vl, int);
	for (i = 1; i < n; i++) {
		val = va_arg(vl, int);
		largest = (largest > val) ? largest : val;
	}
	va_end(vl);
	return largest;
}

#define PP_NARG(...) PP_NARG_(__VA_ARGS__, PP_RSEQ_N()) 
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N(  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
   _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, \
   _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, \
   _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, \
   _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, \
   _63, N, ...) N 
#define PP_RSEQ_N() 63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48, \
  47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26, \
  25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0

#define max(args...) find_max(PP_NARG(args), ##args)

int main()
{
  int m;
  m = max(702, 422, 631, 834, 892, 104, 772);
  printf("The largest value is: %d\n", m);
  return 0;
}