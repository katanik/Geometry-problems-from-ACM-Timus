#include <iostream>
#include <vector>
#include <functional>

using namespace std;

using matrix2D = vector<vector<int> >;
using matrix3D = vector<vector<vector<int> > >;
using matrix4D = vector<vector<vector<vector<int> > > >;

matrix4D g(6, matrix3D(4));
vector<vector<vector<char> > > a, ans;
int n;
bool f = 0;
vector<bool> used(6, 0);

void mark(int& ii, int& jj, int& side, char ch)
{
#define forn(i,p,n) for (int i=p; i<n; i++)

	switch (side)
	{
	case 1:
	{
		forn(i, 0, n) forn(j, 0, n) forn(k, 0, g[ii][jj][i][j])
			a[n - i - 1][j][n - k - 1] = ch;
		break;
	}
	case 2:
	{
		forn(i, 0, n) forn(j, 0, n) forn(k, 0, g[ii][jj][i][j])
			a[n - k - 1][j][i] = ch;
		break;
	}
	case 3:
	{
		forn(i, 0, n) forn(j, 0, n) forn(k, 0, g[ii][jj][i][j])
			a[k][j][n - i - 1] = ch;
		break;
	}
	case 4:
	{
		forn(i, 0, n) forn(j, 0, n) forn(k, 0, g[ii][jj][i][j])
			a[n - i - 1][k][j] = ch;
		break;
	}
	case 5:
	{
		forn(i, 0, n) forn(j, 0, n) forn(k, 0, g[ii][jj][i][j])
			a[n - i - 1][n - k - 1][n - j - 1] = ch;
		break;
	}
	};
}

bool check(int& ii, int& jj, int& side)
{
#define forn(i,p,n) for (int i=p; i<n; i++)

	switch (side)
	{
	case 1:
	{
		forn(i, 0, n) forn(j, 0, n) forn(k, 0, g[ii][jj][i][j])
			if (a[n - i - 1][j][n - k - 1] != '-')
				return false;
		break;
	}
	case 2:
	{
		forn(i, 0, n) forn(j, 0, n) forn(k, 0, g[ii][jj][i][j])
			if (a[n - k - 1][j][i] != '-')
				return false;
		break;
	}
	case 3:
	{
		forn(i, 0, n) forn(j, 0, n) forn(k, 0, g[ii][jj][i][j])
			if (a[k][j][n - i - 1] != '-')
				return false;
		break;
	}
	case 4:
	{
		forn(i, 0, n) forn(j, 0, n) forn(k, 0, g[ii][jj][i][j])
			if (a[n - i - 1][k][j] != '-')
				return false;
		break;
	}
	case 5:
	{
		forn(i, 0, n) forn(j, 0, n) forn(k, 0, g[ii][jj][i][j])
			if (a[n - i - 1][n - k - 1][n - j - 1] != '-')
				return false;
		break;
	}
	};

	return true;
}

void out()
{
	for (int k = 0; k < n; k++)
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				printf("%c ", ans[i][j][k]);
			printf("\n");
		}
}

void brute_force(int i, int j, int side);

void change_side(int side)
{
	for (int i = 0; i < 6; ++i)
	{
		if (used[i])
			continue;

		for (int j = 0; j < 4; ++j)
		{
			brute_force(i, j, side);
		}
	}
}

void brute_force(int i, int j, int side)
{
	if (f || !check(i, j, side))
		return;

	mark(i, j, side, char(i + '1'));
	used[i] = 1;

	if (side >= 5)
	{
		f = 1;
		ans = a;
		return;
	}

	change_side(side + 1);
	used[i] = 0;

	mark(i, j, side, '-'); // back
}

matrix2D orient(const matrix2D& v)
{
	matrix2D new_v = v;
	for (size_t i = 0; i < v.size(); ++i)
		for (size_t j = 0; j < v.size(); ++j)
		{
			new_v[i][j] = v[v.size() - 1 - j][i];
		}

	return new_v;
}

void in(int k)
{
	matrix2D v(n, std::vector<int>(n));
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
		{
			scanf_s("%d", &v[i][j]);
		}

	g[k][0] = v;

	for (int i = 0; i < 3; ++i)
		g[k][i + 1] = orient(g[k][i]);
}


int main()
{
	scanf_s("%d", &n);

	for (int i = 0; i < 6; ++i)
		in(i);

	a.assign(n, vector<vector<char> >(n, vector<char>(n, '-')));

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			for (int k = 0; k < g[0][0][i][j]; ++k)
			{
				a[i][j][k] = char('1');
			}

	used[0] = 1;

	change_side(1);

	if (f)
	{
		printf("Yes\n");
		out();
	}
	else
		printf("No");
}