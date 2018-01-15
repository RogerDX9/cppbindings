#pragma once

#include "TypeInfo.h"
#include "PrimitiveType.h"
#include "StdVectorType.h"
#include "ClassType.h"
#include <vector>

namespace CBind
{

//------------------------------------------------------------
// template specialization for bool
template<>
struct TypeInfo <bool>: public PrimitiveType<bool>
{
    TypeInfo(): PrimitiveType<bool>("bool") {}

    virtual EType getType() const { return EBool; }
};

//------------------------------------------------------------
// template specialization for int
template<>
struct TypeInfo <int>: public PrimitiveType<int>
{
    TypeInfo(): PrimitiveType<int>("int") {}

    virtual EType getType() const { return EInt; }
};

//------------------------------------------------------------
// template specialization for unsigned
template<>
struct TypeInfo <unsigned>: public PrimitiveType<unsigned>
{
    TypeInfo(): PrimitiveType<unsigned>("unsigned") {}

    virtual EType getType() const { return EUInt; }
};

//------------------------------------------------------------
// template specialization for float
template<>
struct TypeInfo <float>: public PrimitiveType<float>
{
    TypeInfo(): PrimitiveType<float>("float") {}

    virtual EType getType() const { return EFloat; }
};

//------------------------------------------------------------
// template specialization for double
template<>
struct TypeInfo <double>: public PrimitiveType<double>
{
    TypeInfo(): PrimitiveType<double>("double") {}

    virtual EType getType() const { return EDouble; }
};

//------------------------------------------------------------
// template specialization for vector
template<typename TElem>
struct TypeInfo<std::vector<TElem>> : public StdVectorType<std::vector<TElem>>
{
    TypeInfo(): StdVectorType<std::vector<TElem>>("std::vector")
    {
    }
};

//------------------------------------------------------------
// global variables
template<typename T>
TypeInfo<T> PrimitiveType<T>::btype;

template<typename T>
TypeInfo<T> StdVectorType<T>::btype;

template<typename T>
TypeInfo<T> ClassType<T>::btype;

}