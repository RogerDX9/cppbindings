#pragma once

#include "..\cppbindings\reflection.h"
#include <cstdio>

void printHierarchy(const ITypeInfo* inType, const void* inInstance, int inTabsCount, const char * inMemberName/* = NULL*/)
{
    for (int idx = 0; idx < inTabsCount; ++idx)
        printf("    ");

    if (inType->getType() == EBool)
    {
        bool v = inType->getValue<bool>(inInstance);
        if (inMemberName)
            printf("%s %s %s", inType->getName(), inMemberName, v ? "true" : "false");
        else
            printf("%s %s", inType->getName(), v ? "true" : "false");
    }
    else if (inType->getType() == EInt)
    {
        int v = inType->getValue<int>(inInstance);
        if (inMemberName)
            printf("%s %s %d", inType->getName(), inMemberName, v);
        else
            printf("%s %d", inType->getName(), v);
    }
    else if (inType->getType() == EUInt)
    {
        unsigned v = inType->getValue<unsigned>(inInstance);
        if (inMemberName)
            printf("%s %s %d", inType->getName(), inMemberName, v);
        else
            printf("%s %d", inType->getName(), v);
    }
    else if (inType->getType() == EDouble)
    {
        double v = inType->getValue<double>(inInstance);
        if (inMemberName)
            printf("%s %s %f", inType->getName(), inMemberName, v);
        else
            printf("%s %f", inType->getName(), v);
    }
    else if (inType->getType() == EClass) // if it's a Class
    {
        const IClassType* classType = inType->getClassType();
        const void* instance = inInstance;

        {
            if (inMemberName)
                printf("%s %s\n", inType->getName(), inMemberName);
            else
                printf("class %s\n", inType->getName());

            const std::vector<IMember*>* members = classType->getMembers();
            for (std::vector<IMember*>::const_iterator it = members->begin(); it != members->end(); ++it)
            {
                const IMember*      m = (*it);
                const void*         instance = m->getValuePtr(inInstance);

                if (!m->isPointer())
                {
                    printHierarchy(m->getTypeInfo(), instance, inTabsCount + 1, m->getName());
                }
                else
                {
                    for (int idx = 0; idx < inTabsCount + 1; ++idx)
                        printf("    ");

                    printf("%s %s %p", m->getTypeInfo()->getName(), m->getName(), instance);
                    printf("\n");

                    for (int idx = 0; idx < inTabsCount; ++idx)
                        printf("    ");
                }
            }
        }
    }
    else if (inType->getType() == EArray)
    {
        const IArrayType* arrayType = inType->getArrayType();

        if (inMemberName)
            printf("%s<%s> %s\n", arrayType->getName(), arrayType->getElementType()->getName(), inMemberName);
        else
            printf("%s<%s>\n", arrayType->getName(), arrayType->getElementType()->getName());

        const void* arrayInstance = inInstance;
        size_t size = arrayType->getArrayCount(arrayInstance);

        for (size_t idx = 0; idx < size; ++idx)
        {
            const void* valueInstance = arrayType->getArrayValuePtr(arrayInstance, idx);
            printHierarchy(arrayType->getElementType(), valueInstance, inTabsCount + 1, NULL);
        }
    }
    else
    {
        printf("%s", inType->getName());
    }
    printf("\n");
}

template<class T>
void printHierarchy(T& inObject)
{
    printHierarchy(&TypeInfo<T>::btype, &inObject, 0, NULL);
}