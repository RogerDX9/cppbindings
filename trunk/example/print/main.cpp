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

#include <cstdio>

void printHierarchy(const ITypeInfo* inType, const void* inInstance, int inTabsCount)
{
	for (int idx = 0; idx < inTabsCount; ++idx)
		printf("    ");

	if (inType->getType() == EBool)
	{
		bool v = inType->getValue<bool>(inInstance);
		if (inType->isMember())
			printf("%s %s %s", inType->getName(), inType->getMemberName(), v ? "true" : "false");
		else
			printf("%s %s", inType->getName(), v ? "true" : "false");
	}
	else if (inType->getType() == EInt)
	{
		int v = inType->getValue<int>(inInstance);
		if (inType->isMember())
			printf("%s %s %d", inType->getName(), inType->getMemberName(), v);
		else
			printf("%s %d", inType->getName(), v);
	}
	else if (inType->getType() == EUInt)
	{
		unsigned v = inType->getValue<unsigned>(inInstance);
		if (inType->isMember())
			printf("%s %s %d", inType->getName(), inType->getMemberName(), v);
		else
			printf("%s %d", inType->getName(), v);
	}
	else if (inType->getType() == EDouble)
	{
		double v = inType->getValue<double>(inInstance);
		if (inType->isMember())
			printf("%s %s %f", inType->getName(), inType->getMemberName(), v);
		else
			printf("%s %f", inType->getName(), v);
	}
	else if (inType->getType() == EClass) // if it's a Class
	{
		const void* instance = inType->getValuePtr(inInstance);

		if (inType->isPointer())
		{
			printf("%s* %s %p", inType->getName(), inType->getMemberName(), instance);
		}
		else
		{
			if (inType->isMember())
				printf("%s %s\n", inType->getName(), inType->getMemberName());
			else
				printf("class %s\n", inType->getName());

			const std::vector<ITypeInfo*>* members = inType->getMembers();
			for (std::vector<ITypeInfo*>::const_iterator it = members->begin(); it != members->end(); ++it)
			{
				printHierarchy(*it, instance, inTabsCount + 1);
			}
		}
	}
	else if (inType->getType() == EArray)
	{
		printf("%s<%s> %s\n", inType->getName(), inType->getElementType()->getName(), inType->getMemberName());

		const void* arrayInstance = inType->getValuePtr(inInstance);
		size_t size = inType->getArrayCount(arrayInstance);

		for (size_t idx = 0; idx < size; ++idx)
		{
			const void* valueInstance = inType->getArrayValuePtr(arrayInstance, idx);
			printHierarchy(inType->getElementType(), valueInstance, inTabsCount + 1); 
		}
	}
	else
	{
		printf("%s %s", inType->getName(), inType->getMemberName());
	}
	printf("\n");
}

template<class T>
void printHierarchy(T& inObject)
{
	TypeInfo<T> type;
	printHierarchy(&type, &inObject, 0);
}

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