/*Copyright (C) 2014 Dmytro Shchukin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

#ifdef _MSC_VER
#include "..\..\build\vs2012\stdafx.h"
#endif

#include "..\Car.h"
#include "..\Wheel.h"
#include "..\Tools.h"


#ifdef _MSC_VER
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
	Car car;
	car.m_speed = 1;
	car.m_id = 777;
	car.m_mass = 1075;
	car.m_wheelPtr = NULL;
	car.m_wheel.m_rotation = 15;
	car.m_wheel.m_screws.push_back(1);
	car.m_wheel.m_screws.push_back(2);
	car.m_wheel.m_screws.push_back(3);
	car.m_wheel.m_screws.push_back(4);
	car.m_wheels.push_back(car.m_wheel);

	printHierarchy(car);

	return 0;
}