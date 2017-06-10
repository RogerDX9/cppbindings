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

    printBindings(car);

    return 0;
}