// Bindings.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Reflection.h"
#include "Car.h"
#include "Wheel.h"

void printHierarchy(const ITypeInfo* inType, const void* inInstance, int inTabsCount)
{
	for (int idx = 0; idx < inTabsCount; ++idx)
		printf("    ");

	if (inType->getType() == EClass) // if it's a Class
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
	else if (inType->getType() == EInt)
	{
		int v = inType->getValue<int>(inInstance);
		//inType->setValue<int>(inInstance, v + 5);
		if (inType->isMember())
			printf("%s %s %d", inType->getName(), inType->getMemberName(), v);
		else
			printf("%s %d", inType->getName(), v);
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

int _tmain(int argc, _TCHAR* argv[])
{
	Car car;
	TypeInfo<Car> carType;

	printHierarchy(&carType, &car, 0);

	return 0;
}