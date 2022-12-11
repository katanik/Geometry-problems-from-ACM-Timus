#pragma comment (linker, "/STACK:67108864")
#include<iostream>

using namespace std;

const double eps = 1e-9, pi = acos(-1.);

struct Point
{
	double x, y;

	Point() = default;

	Point(double x, double y) : x(x), y(y)
	{}

	Point operator+ (Point p)
	{
		return Point(x + p.x, y + p.y);
	}

	Point operator- (Point p)
	{
		return Point(x - p.x, y - p.y);
	}

	Point operator* (double p)
	{
		return Point(x * p, y * p);
	}

	double operator* (Point p)
	{
		return x * p.x + y * p.y;
	}

	Point turn(double ang, Point O)
	{
		return Point(O.x + (x - O.x) * cos(ang) + (y - O.y) * sin(ang), O.y - (x - O.x) * sin(ang) + (y - O.y) * cos(ang));
	}

	double mod()
	{
		return sqrt(x * x + y * y);
	}
};

double radius1, radius2, radius3, radius4;
Point pointA, pointB, pointC;

double f(double ang, Point& p1)
{
	Point p = (Point(pointB.x - radius4, pointB.y)).turn(ang, pointB);
	return (p1 - p).mod() + (p - pointA).mod();
}

double ternarSubSearch(double ang)
{
	Point p = (Point(pointC.x + radius4, pointC.y)).turn(ang, pointC);
	double l = 0.0;
	double r = acos(((pointA - pointB) * (p - pointB)) / (pointA - pointB).mod() / (p - pointB).mod());
	double m1 = 0.0, m2 = 0.0;

	while (r - l > eps)
	{
		m1 = l + (r - l) / 3.0;
		m2 = r - (r - l) / 3.0;
		if (f(m1, p) < f(m2, p)) r = m2;
		else l = m1;
	}
	return (pointA - p).mod() + f((r + l) / 2.0, p);
}

double ternarSearch()
{
	double l = 0, r = pi, m1, m2;
	while ((r - l) > eps)
	{
		m1 = l + (r - l) / 3.0;
		m2 = r - (r - l) / 3.0;
		if (ternarSubSearch(m1) < ternarSubSearch(m2)) r = m2;
		else l = m1;
	}

	return ternarSubSearch((l + r) * 0.5);
}

double find_x()
{
	return (radius1 * radius1 + radius2 * radius2 - radius3 * radius3) / (2. * radius1);
}

double find_h(double x)
{
	return sqrt(radius2 * radius2 - x * x);
}

int main()
{
	scanf_s("%lf%lf%lf%lf", &radius1, &radius2, &radius3, &radius4);
	pointA = Point(0, 0);
	double x = find_x(), h = find_h(x);
	pointB = Point(radius1, 0); pointC = Point(x, h);

	if (radius1 <= radius4 && radius2 > radius4)
	{
		printf("%.3f", 2.0 * (radius2 - radius4));
		return 0;
	}

	if (radius2 <= radius4 && radius1 > radius4)
	{
		printf("%.3f", 2.0 * (radius1 - radius4));
		return 0;
	}

	if (radius1 <= radius4 && radius2 <= radius4)
	{
		printf("%.3f", 0.0);
		return 0;
	}

	printf("%.3f", ternarSearch());
}