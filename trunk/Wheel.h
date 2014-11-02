#ifndef WHEEL_H
#define WHEEL_H

#include "Reflection.h"

class Wheel
{
	int					m_rotation;
	std::vector<int>	m_screws;

public:

	Wheel()
	{
		m_rotation = 15;
		m_screws.push_back(1);
		m_screws.push_back(2);
		m_screws.push_back(3);
		m_screws.push_back(4);
	}

	static std::string bindReflection( IClassType * inReflection )
	{
		inReflection->bind("m_rotation",	&Wheel::m_rotation);
		inReflection->bind("m_screws",		&Wheel::m_screws);

		return "Wheel";
	}
};

template<> struct TypeInfo <Wheel>: public ClassType<Wheel> {};
#endif
