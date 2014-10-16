#ifndef CAR_H
#define CAR_H

#include "Reflection.h"
#include "Wheel.h"
#include <vector>

class Car
{
	Wheel m_wheel;
	Wheel* m_pWheel;
	std::vector<int> m_gears;

public:
	int m_speed;

	static void bindReflection(IClassType * inReflection)
	{
		inReflection->bind("m_speed", &Car::m_speed);
		inReflection->bind("m_wheel", &Car::m_wheel);
		inReflection->bind("m_pWheel", &Car::m_pWheel);
		inReflection->bind("m_gears", &Car::m_gears);
	}
};

template<>
struct Type <Car>: public ClassType<Car>
{
	Type(): ClassType<Car>("Car") {}
};

#endif