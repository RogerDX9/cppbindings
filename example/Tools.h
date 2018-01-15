#pragma once

#include "..\cppbindings\reflection.h"
#include <cstdio>

void printBindings(const CBind::ITypeInfo* inType, const void* inInstance, int inTabsCount, const char * inMemberName/* = NULL*/)
{
    for (int idx = 0; idx < inTabsCount; ++idx)
        printf("    ");

    switch (inType->getType())
    {
    case CBind::EBool:
    {
        bool v = inType->getValue<bool>(inInstance);
        if (inMemberName)
            printf("%s %s %s", inType->getName(), inMemberName, v ? "true" : "false");
        else
            printf("%s %s", inType->getName(), v ? "true" : "false");
        break;
    }
    case CBind::EInt:
    {
        int v = inType->getValue<int>(inInstance);
        if (inMemberName)
            printf("%s %s %d", inType->getName(), inMemberName, v);
        else
            printf("%s %d", inType->getName(), v);
        break;
    }
    case CBind::EUInt:
    {
        unsigned v = inType->getValue<unsigned>(inInstance);
        if (inMemberName)
            printf("%s %s %d", inType->getName(), inMemberName, v);
        else
            printf("%s %d", inType->getName(), v);
        break;
    }
    case CBind::EDouble:
    {
        double v = inType->getValue<double>(inInstance);
        if (inMemberName)
            printf("%s %s %f", inType->getName(), inMemberName, v);
        else
            printf("%s %f", inType->getName(), v);
        break;
    }
    case CBind::EClass:
    {
        if (inMemberName)
            printf("%s %s\n", inType->getName(), inMemberName);
        else
            printf("class %s\n", inType->getName());

        const void*                         classInstance = inInstance;
        const CBind::IClassType*            classType = inType->getClassType();
        const std::vector<CBind::IMember*>* members = classType->getMembers();
        for (std::vector<CBind::IMember*>::const_iterator it = members->begin(); it != members->end(); ++it)
        {
            const CBind::IMember*   m = (*it);
            const void*             memberPtr = m->getPtr(classInstance);
            const char*             memberName = m->getName();
            const CBind::ITypeInfo* memberType = m->getTypeInfo();

            if (!m->isPointer())
            {
                printBindings(memberType, memberPtr, inTabsCount + 1, memberName);
            }
            else
            {
                for (int idx = 0; idx < inTabsCount + 1; ++idx)
                    printf("    ");

                printf("%s %s %p", memberType->getName(), memberName, memberPtr);
                printf("\n");

                for (int idx = 0; idx < inTabsCount; ++idx)
                    printf("    ");
            }
        }
        break;
    }
    case CBind::EArray:
    {
        const void*                 arrayInstance = inInstance;
        const CBind::IArrayType*    arrayType = inType->getArrayType();
        const CBind::ITypeInfo*     elementType = arrayType->getElementType();
        size_t                      size = arrayType->getCount(arrayInstance);

        if (inMemberName)
            printf("%s<%s> %s\n", arrayType->getName(), elementType->getName(), inMemberName);
        else
            printf("%s<%s>\n", arrayType->getName(), elementType->getName());

        for (size_t idx = 0; idx < size; ++idx)
        {
            const void* valueInstance = arrayType->getValuePtr(arrayInstance, idx);
            printBindings(elementType, valueInstance, inTabsCount + 1, NULL);
        }
        break;
    }
    default:
        printf("%s", inType->getName());
        break;
    }

    printf("\n");
}

//------------------------------------------------------------
template<class T>
void printBindings(T& inObject)
{
    printBindings(&CBind::TypeInfo<T>::btype, &inObject, 0, NULL);
}