#pragma once

#include <string>
#include <assert.h>

namespace CBind
{

//------------------------------------------------------------
enum EType
{
    ENotImplemented,
    EBool,
    EInt,
    EUInt,
    EFloat,
    EDouble,
    EString,
    EClass,
    EArray
};

//------------------------------------------------------------
struct IClassType;
struct IArrayType;

//------------------------------------------------------------
struct ITypeInfo
{
    ITypeInfo(const std::string & inName)
        : m_name(inName)
    {
    }

    virtual ~ITypeInfo() {}

    virtual EType               getType() const         { return ENotImplemented; }
    const char*                 getName() const         { return m_name.c_str(); }
    virtual const IClassType*   getClassType() const    { return NULL; }
    virtual const IArrayType*   getArrayType() const    { return NULL; }

    virtual const void*         getValuePtr(const void* instance) const { return instance; }
    virtual void                setValuePtr(const void* instance, const void* value) const { assert(false); }

    template<typename T>
    void setValue(const void* instance, const T & inValue) const
    {
        const void* val = &inValue;
        setValuePtr(instance, val);
    }

    template<typename T>
    const T & getValue(const void* instance) const
    {
        const T* val = (T*)getValuePtr(instance);
        return *val;
    }
protected:
    std::string m_name;
};

//------------------------------------------------------------
template<typename T>
struct TypeInfo : public ITypeInfo
{
    static TypeInfo<T> btype;
};

} // CBind