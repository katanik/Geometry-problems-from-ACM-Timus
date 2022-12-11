#include <iostream>
#include <vector>

struct Point
{
	Point() = default;
	Point(double inX, double inY, double inZ)
		: x(inX), y(inY), z(inZ)
	{}

	Point operator^ (Point p) const
	{
		return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
	}

	double operator* (Point p) const
	{
		return x * p.x + y * p.y + z * p.z;
	}

	bool operator== (Point p) const
	{
		return x == p.x && y == p.y && z == p.z;
	}

	bool operator!= (Point p) const
	{
		return !(*this == p);
	}

	double mod() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	bool in()
	{
		scanf_s("%lf%lf%lf", &x, &y, &z); return 1;
	}

private:
	double x, y, z;
};

int circle(std::vector <Point>& v, Point p)
{
	int k1 = 0, k2 = 0;
	for (size_t i = 0; i < v.size(); i++)
	{
		double d = p * v[i];
		if (d > 0) k1++;
		if (d < 0) k2++;
	}

	return std::max(k1, k2);
}

int count(Point& a, Point& b, const std::vector<Point>& points, std::vector<Point>& used)
{
	Point norm = a ^ b;
	for (const auto& pt : used)
	{
		if ((pt ^ norm).mod() == 0)
			return 0;
	}

	used.push_back(norm);
	int k1 = 0, k2 = 0, mx = 0;
	std::vector <Point> v;
	for (const auto& pt : points)
	{
		double p = (a ^ b) * pt;
		if (p > 0)
		{
			k1++;
			continue;
		}

		if (p < 0)
		{
			k2++;
			continue;
		}
		v.push_back(pt);
	}

	for (const auto& pt : v)
	{
		mx = std::max(mx, circle(v, ((a ^ b) ^ pt)));
	}

	return std::max(k1, k2) + mx + 1;
}

int main()
{
	int mx = 0;
	int n = 0;
	scanf_s("%d", &n);

	std::vector<Point> points, used;
	points.resize(n);
	for (auto& pt : points)
	{
		pt.in();
	}

	for (int i = 0; i < n - 1; ++i)
	{
		for (int j = i + 1; j < n; ++j)
		{
			if ((points[i] ^ points[j]).mod() != 0)
				mx = std::max(mx, count(points[i], points[j], points, used));
		}
	}

	printf("%d", std::max(1, mx));
}
