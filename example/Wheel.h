#pragma once 

#include "..\cppbindings\reflection.h"

struct Wheel
{
	int					m_rotation;
	std::vector<int>	m_screws;

	static std::string bindReflection(ClassType<Wheel> * inReflection)
	{
		inReflection->bind("m_rotation",	&Wheel::m_rotation);
		inReflection->bind("m_screws",		&Wheel::m_screws);

		return "Wheel";
	}
};

// Tamplate specification
template<> struct TypeInfo <Wheel>: public ClassType<Wheel> {};
