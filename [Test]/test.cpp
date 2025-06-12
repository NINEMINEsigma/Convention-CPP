//#include"Config.hpp"

using namespace std;

#include <iostream>
#include<vector>
#include<map>
using namespace std;

using ll = long long;
int n = 0;
const int N = 1e6 + 2;
ll a[N], f[N];
int main()
{
	// 请在此输入您的代码
	map<int, int> flag;
	cin >> n;
	f[1] = 1;
	f[2] = 1;
	for (int i = 3; i <= n; i++)
	{
		f[i] = f[i - 1] + f[i - 2];
	}
	for (int i = 1; i <= n; i++)
	{
		int temp = 0;
		cin >> temp;
		if (temp % f[i] == 0)
		{
			flag[temp / f[i]]++;
		}
	}
	int maxs = 0;
	for (auto i = flag.begin(); i != flag.end(); ++i)
	{
		maxs = max(maxs, i->second);
	}
	cout << n - maxs;
	return 0;
}
