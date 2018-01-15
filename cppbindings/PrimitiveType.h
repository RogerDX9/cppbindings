#pragma once

#include "TypeInfo.h"

namespace CBind
{

//------------------------------------------------------------
template<typename T>
struct PrimitiveType: public ITypeInfo
{
    static TypeInfo<T> btype;

    PrimitiveType(const std::string & inName)
        : ITypeInfo(inName)
    {
    }

    virtual void setValuePtr(const void* instance, const void* value) const
    {
        T * vPtr = (T*)value;
        T * iPtr = (T*)instance;
        *iPtr = *vPtr;
    }
};

}