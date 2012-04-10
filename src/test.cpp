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
	cout << (v1 + v2) << endl;
	cout << Value<UnitSI<1, -2, 5>>(5000) << endl;
	
	Meter meter;
	Second second;
	Speed u = Value<SpeedUnit>(5);
	return 0;
}
