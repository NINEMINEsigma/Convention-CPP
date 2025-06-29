//#include"Config.hpp"
#include"Architecture.hpp"

using namespace std;

#include <iostream>
#include<vector>
#include<map>
using namespace std;

int main()
{
	double t = 0;
	for (int i = 1; i < 80; i++)
	{
		t += 1.0 / (double)i;
	}
	double k = 0;
	for (int i = 1; i < 80; i++)
	{
		k += 1.0 / (double)i;
		cout << i << ":\t" << k << "/" << t << "\t=\t" << k / t * 100 << "% current step:\t" <<
			1.0 / (double)i << endl;
	}
	return 0;
}
