#pragma once

#include "TypeInfo.h"

namespace CBind
{

//------------------------------------------------------------
struct IMember
{
    IMember(const std::string& inName)
        : m_name(inName)
    {}

    virtual const ITypeInfo*    getTypeInfo() const = 0;

    const char*                 getName() const             { return m_name.c_str(); }
    virtual bool                isPointer() const           { return false; }

    virtual const void*         getPtr(const void* instance) const = 0;

protected:
    std::string         m_name;
};

//------------------------------------------------------------
template<typename T, class U, typename TType>
struct MemberInfo: public IMember
{
    MemberInfo(const std::string& inName, T U::* pMember)
        : IMember   (inName)
        , m_pMember (pMember)
    {
    }

    virtual const ITypeInfo* getTypeInfo() const { return &TypeInfo<TType>::btype; }

    virtual const void* getPtr(const void* inClassInstance) const
    {
        U * iPtr = (U*)inClassInstance;
        return &(iPtr->*m_pMember);
    }

    virtual void setValuePtr(const void* inClassInstance, const void* value) const
    {
        U * iPtr = (U*)inClassInstance;
        T * vPtr = (T*)value;
        
        iPtr->*m_pMember = *vPtr;
    }

protected:
    T U::* m_pMember;
};

//------------------------------------------------------------
template<typename T, class U, typename TType>
struct MemberInfoPtr: public MemberInfo<T, U, TType>
{
    MemberInfoPtr(const std::string& inName, T U::* pMember)
        : MemberInfo<T, U, TType>(inName, pMember)
    {
    }

    virtual bool isPointer() const { return true; }

    virtual const void* getPtr(const void* inClassInstance) const
    {
        U * iPtr = (U*)inClassInstance;
        T U::* mPtr = this->m_pMember;
        return iPtr->*mPtr;
    }

    virtual void setValuePtr(const void* inClassInstance, const void* value) const
    {
        assert(false);
        /*U * iPtr = (U*)inClassInstance;
        T * vPtr = (T*)value;
        iPtr->*m_pMember = *vPtr;*/
    }
};

}