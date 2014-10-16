#ifndef WHEEL_H
#define WHEEL_H

#include "Reflection.h"

class Wheel
{
	int m_rot;
public:

	static void bindReflection( IClassType * inReflection )
	{
		inReflection->bind("m_rot", &Wheel::m_rot);
	}
};

template<>
struct Type <Wheel>: public ClassType<Wheel>
{
	Type(): ClassType<Wheel>("Wheel") {}
};

#endif