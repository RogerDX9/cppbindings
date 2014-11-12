#ifndef TOOLS_H
#define TOOLS_H

#include "..\cppbindings\reflection.h"
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

#endif