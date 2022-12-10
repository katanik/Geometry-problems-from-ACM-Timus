#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <functional>
#include <algorithm>
#include <math.h>

using namespace std;

const double eps = 1e-9;
double X, Y;

struct Point {
	int x, y;
	Point(int x = 0, int y = 0) : x(x), y(y) {}

	Point operator- (Point p)
	{
		return Point(x - p.x, y - p.y);
	}

	// cross product in 2D
	int operator^ (Point p)
	{
		return x * p.y - y * p.x;
	}

	void in()
	{
		scanf_s("%d%d", &x, &y);
	}
};

struct Segment {
	Point start, end;
	Segment() {}
	Segment(Point a, Point b) : start(a), end(b) {}

	~Segment() {}

	double y()
	{
		if (end.x == start.x) return (Y >= min(end.y, start.y) && Y <= max(end.y, start.y)) ? Y : start.y;
		return static_cast<double>(start.y) + static_cast<double>((end.y - start.y) * (X - start.x)) / static_cast<double>(end.x - start.x);
	}

	bool operator< (Segment s)
	{
		return (*this).y() < s.y();
	}

	bool operator<= (Segment s)
	{
		return (*this).y() <= s.y();
	}

	bool operator== (Segment s)
	{
		return fabs((*this).y() - s.y()) < eps;
	}

	bool doesIntersect(Segment s)
	{
		if (!areCoincidence(start.x, end.x, s.start.x, s.end.x) || !areCoincidence(start.y, end.y, s.start.y, s.end.y))
			return false;

		return ((s.end - start) ^ (s.start - s.end)) * 1ll * ((s.end - end) ^ (s.start - s.end)) <= 0
			&& ((end - s.start) ^ (start - end)) * 1ll * ((end - s.end) ^ (start - end)) <= 0;
	}

	void in()
	{
		start.in(); end.in();
		if (start.x > end.x)
			swap(start, end);
	}

private:
	bool areCoincidence(int a, int b, int c, int d)
	{
		if (a > b)
			swap(a, b);
		if (c > d)
			swap(c, d);
		if (a > c)
		{
			swap(a, c);
			swap(b, d);
		}
		return c >= a && c <= b;
	}
};

struct Event
{
	int p;
	int tp, id;
	Event(int p, int tp, int id) : p(p), tp(tp), id(id) {}
	bool operator< (Event e) { return p == e.p ? tp > e.tp : p < e.p; }
};


vector <Segment> segments;
vector <Event> events;

// Cartesian tree
class Treap
{
	int x, y;
	Segment first, last;
	Treap* l, * r;
public:
	Treap(int x = 0, int y = 0);
	~Treap();
	friend void merge(Treap*&, Treap*, Treap*);
	friend void split(int, Treap*, Treap*&, Treap*&);
	friend void erase(int x, Treap*&);
	int get_left();
	int get_right();
	void out(int l, int r) const;
};

Treap::Treap(int x, int y) : x(x), y(y), l(0), r(0)
{}

void merge(Treap*& T, Treap* L, Treap* R)
{
	if (!L)
	{
		T = R;
		return;
	}
	if (!R)
	{
		T = L;
		return;
	}
	if (L->y <= R->y)
	{
		merge(R->l, L, R->l);
		T = R;
	}
	else {
		merge(L->r, L->r, R);
		T = L;
	}
}

void split(int x, Treap* T, Treap*& L, Treap*& R)
{
	if (!T)
	{
		L = nullptr;
		R = nullptr;
		return;
	}
	if (segments[T->x] <= segments[x])
	{
		split(x, T->r, T->r, R);
		L = T;
	}
	else
	{
		split(x, T->l, L, T->l);
		R = T;
	}
}

int Treap::get_left()
{
	if (!this) return -1;
	Treap* cur = this;
	for (; cur->r; cur = cur->r);
	return cur->x;
}

int Treap::get_right()
{
	if (!this) return -1;
	Treap* cur = this;
	for (; cur->l; cur = cur->l);
	return cur->x;
}

void Treap::out(int l, int r) const
{
	printf("YES\n%d %d", min(l + 1, r + 1), max(l + 1, r + 1));
	exit(0);
}

void insert(int x, Treap*& T)
{
	Treap* t, * L, * R;
	t = new Treap(x, rand());
	split(x, T, L, R);
	int l = L->get_left(), r = R->get_right();
	if (l != -1 && l != x && segments[l].doesIntersect(segments[x]))
		T->out(l, x);
	if (r != -1 && r != x && segments[r].doesIntersect(segments[x]))
		T->out(r, x);
	merge(L, L, t);
	merge(T, L, R);
}

void erase(int x, Treap*& T)
{
	if (!T) return;
	if (segments[x] == segments[T->x])
		merge(T, T->l, T->r);
	else
		erase(x, segments[x] <= segments[T->x] ? T->l : T->r);
}

void solve()
{
	Treap* pTreap = nullptr;
	sort(events.begin(), events.end());

	for (int i = 0; i < static_cast<int>(events.size()); i++)
	{
		int x = events[i].id;
		X = events[i].p;
		Y = events[i].tp == 1 ? segments[x].start.y : segments[x].end.y;

		if (events[i].tp == 1)
			insert(x, pTreap);
		else {
			erase(x, pTreap);
			Treap* L, * R;
			split(x, pTreap, L, R);
			int l = L->get_left();
			int r = R->get_right();
			if (l != -1 && r != -1 && l != x && r != x && segments[l].doesIntersect(segments[r]))
				pTreap->out(l, r);

			merge(pTreap, L, R);
		}
	}
	printf("NO");
}

int main() {
	int n;
	scanf_s("%d", &n);
	segments.resize(n);
	for (int i = 0; i < n; i++)
	{
		segments[i].in();
		events.push_back(Event(segments[i].start.x, 1, i));
		events.push_back(Event(segments[i].end.x, -1, i));
	}
	solve();
}
