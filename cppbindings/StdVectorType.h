#pragma once

#include "ArrayType.h"

namespace CBind
{

//------------------------------------------------------------
template<typename TContainer>
struct StdVectorType : public IArrayType
{
    static TypeInfo<TContainer> btype;

    StdVectorType(const std::string & inName)
        : IArrayType(inName)
    {
    }

    virtual const IArrayType* getArrayType() const { return &btype; }

    virtual const ITypeInfo* getElementType() const
    {
        return &TypeInfo<typename TContainer::value_type>::btype;
    }

    virtual const size_t getCount(const void* instance) const
    {
        const TContainer* array = (const TContainer*)instance;
        return array->size();
    }

    virtual const void* getValuePtr(const void* instance, int inIndex) const
    {
        const TContainer* array = (const TContainer*)instance;
        const void* value = &array->at(inIndex);
        return value;
    }

    virtual void resize(const void* instance, size_t n) const
    {
        TContainer* array = (TContainer*)instance;
        array->resize(n);
    }
};

}