#ifndef WHEEL_H
#define WHEEL_H

#include "Reflection.h"

struct Wheel
{
	int					m_rotation;
	std::vector<int>	m_screws;

	static std::string bindReflection( IClassType * inReflection )
	{
		inReflection->bind("m_rotation",	&Wheel::m_rotation);
		inReflection->bind("m_screws",		&Wheel::m_screws);

		return "Wheel";
	}
};

// Tamplate specification for Wheel
template<> struct TypeInfo <Wheel>: public ClassType<Wheel> {};
#endif
