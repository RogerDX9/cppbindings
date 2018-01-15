#pragma once

#include "TypeInfo.h"

namespace CBind
{

//------------------------------------------------------------
struct IArrayType : public ITypeInfo
{
    IArrayType(const std::string & inName)
        : ITypeInfo(inName)
    {
    }

    virtual EType               getType() const { return EArray; }

    virtual const ITypeInfo*    getElementType() const = 0;
    virtual const size_t        getCount(const void* instance) const = 0;
    virtual const void*         getValuePtr(const void* instance, int inIndex) const = 0;
    virtual void                resize(const void* instance, size_t n) const = 0;
};

}