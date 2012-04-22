#include "si.h"

int main()
{
//	double d;
//	cin >> d;
	Speed v1 = 10.m/2.5s;
	Speed v2 = 5.m/2.5s;
	Time dist = (v1+v2) * 10.s / v2;
	cout << v1 << endl;
	cout << v2 << endl;
	cout << 10.m << endl;
	cout << 20.s << endl;
	cout << dist << endl;
	Speed sum_v = v1 + v2;
	cout << (v1 + v2) / 2.s << endl;
	cout << AngularAcceleration(5000) << endl;
	return 0;
}
