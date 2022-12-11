#include<iostream>
#include<cmath>

const int N = 8;
const double pi = acos(-1.);

double arccos(double x)
{
	x = std::min(1.0, std::max(-1.0, x));
	return acos(x);
}

// point in N-dimensional space
class Point
{
public:
	Point operator- (Point p)
	{
		Point ans;
		for (int i = 0; i < N; i++) ans.x[i] = x[i] - p.x[i];
		return ans;
	}

	double operator* (Point p)
	{
		double ans = 0;
		for (int i = 0; i < N; i++) ans += x[i] * p.x[i];
		return ans;
	}

	double operator^ (Point p)
	{
		return arccos((*this * p) / ((*this).mod() * p.mod()));
	}

	double mod()
	{
		double ans = 0;
		for (int i = 0; i < N; i++) ans += x[i] * x[i];
		return sqrt(ans);
	}

	void in()
	{
		for (int i = 0; i < N; i++)
			std::cin >> x[i];
	}

private:
	double x[N];
};

double h(double a, double b, double c)
{
	double d = 0.5 * (a * a - b * b + c * c) / c;
	return sqrt(a * a - d * d);
}

int main()
{
	Point pointA, pointB, center;
	pointA.in();
	pointB.in();
	center.in();

	double radius;
	std::cin >> radius;

	double result = 0.0;
	double segmentAC = (pointA - center).mod();
	double segmentBC = (pointB - center).mod();
	double segmentAB = (pointA - pointB).mod();

	if (((pointA - center) ^ (pointA - pointB)) >= pi / 2 ||
		((pointB - center) ^ (pointB - pointA)) >= pi / 2 ||
		radius <= h(segmentAC, segmentBC, segmentAB))
	{
		result = (pointA - pointB).mod();
	}
	else
	{
		result = (((center - pointA) ^ (center - pointB)) - arccos(radius / (center - pointB).mod()) - arccos(radius / (center - pointA).mod())) * radius +
			sqrt(segmentAC * segmentAC - radius * radius) + sqrt(segmentBC * segmentBC - radius * radius);
	}

	printf("%.10f", result);
}