#ifndef CAR_H
#define CAR_H

#include "..\cppbindings\reflection.h"
#include "Wheel.h"
#include <vector>

struct Car
{
	bool				m_automaticTransmission;
	int					m_speed;
	unsigned			m_id;
	double				m_mass;
	Wheel*				m_wheelPtr;
	Wheel				m_wheel;
	std::vector<Wheel>	m_wheels;

	static std::string bindReflection(IClassType * inReflection)
	{
		inReflection->bind("m_automaticTransmission",	&Car::m_automaticTransmission);
		inReflection->bind("m_speed",					&Car::m_speed);
		inReflection->bind("m_id",						&Car::m_id);
		inReflection->bind("m_mass",					&Car::m_mass);
		inReflection->bind("m_wheelPtr",				&Car::m_wheelPtr);
		inReflection->bind("m_wheel",					&Car::m_wheel);
		inReflection->bind("m_wheels",					&Car::m_wheels);

		return "Car";
	}
};

// Tamplate specification for Car
template<> struct TypeInfo <Car>: public ClassType<Car> {};
#endif