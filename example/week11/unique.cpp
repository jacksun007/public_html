#include <memory>
#include <iostream>

using namespace std;

struct Point
{
	int x, y;
	Point(int x, int y) : x(x), y(y) {}
};

ostream & operator<<(ostream & os, const Point & pt)
{
	os << "(" << pt.x << ", " << pt.y << ")";
	return os;
}

void foo(unique_ptr<Point> & giver)
{
	unique_ptr<Point> taker = std::move(giver);
	cout << "Took over " << *taker << endl;
}

int main()
{
	unique_ptr<Point> point(new Point(1, 2)); 
	foo(point);
	if (point) {
		cout << "main still owns point" << endl;
	} else {
		cout << "point is null" << endl;
	}

	return 0;
}