// Bindings.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Reflection.h"
#include "Car.h"
#include "Wheel.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Car car;
	car.m_speed = 1;

	Type<Car> t;

	printf("Car speed is %d \n", car.m_speed);
	t.setValue("m_speed", car, 5);
	printf("Car speed is %d \n", car.m_speed);

	printf("Car.m_speed type is %s \n", t.getMember("m_speed")->m_name.c_str());
	printf("Car.m_wheel type is %s \n", t.getMember("m_wheel")->m_name.c_str());
	printf("Car.m_pWheel type is %s \n", t.getMember("m_pWheel")->m_name.c_str());
	printf("Car.m_gears type is %s<%s> \n", t.getMember("m_gears")->m_name.c_str(),
		t.getMember("m_gears")->getElementType()->m_name.c_str());

	printf("Car.m_pWheel type is a pointer: %s \n", t.getMember("m_pWheel")->isPointer() ? "yes" : "no");
	printf("Car.m_wheel type is a pointer: %s \n", t.getMember("m_wheel")->isPointer() ? "yes" : "no");

	IType* tWheel = t.getMember("m_wheel");
	printf("Wheel.m_rot type is %s \n", tWheel->getMember("m_rot")->m_name.c_str());

	return 0;
}