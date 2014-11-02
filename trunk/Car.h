#ifndef CAR_H
#define CAR_H

#include "Reflection.h"
#include "Wheel.h"
#include <vector>

class Car
{
	int					m_speed;
	Wheel*				m_wheelPtr;
	Wheel				m_wheel;
	std::vector<Wheel>	m_wheels;

public:

	Car()
	{
		m_speed = 1;
		m_wheelPtr = NULL;
		m_wheels.push_back(Wheel());
	}

	static std::string bindReflection(IClassType * inReflection)
	{
		inReflection->bind("m_speed",		&Car::m_speed);
		inReflection->bind("m_wheelPtr",	&Car::m_wheelPtr);
		inReflection->bind("m_wheel",		&Car::m_wheel);
		inReflection->bind("m_wheels",		&Car::m_wheels);

		return "Car";
	}
};

template<> struct TypeInfo <Car>: public ClassType<Car> {};

#endif