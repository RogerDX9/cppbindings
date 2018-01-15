#pragma once

#include "TypeInfo.h"
#include "MemberInfo.h"
#include <vector>

namespace CBind
{

//------------------------------------------------------------
struct IClassType: public ITypeInfo
{
    IClassType(const std::string & inName)
        : ITypeInfo(inName)
    {
    }

    ~IClassType()
    {
        for (std::vector<IMember*>::const_iterator it = m_members.begin(); it != m_members.end(); ++it)
        {
            delete (*it);
        }
    }

    virtual EType getType() const
    {
        return EClass;
    }

    virtual const std::vector<IMember*>*    getMembers() const { return &m_members; }

    template<typename T, class U>
    MemberInfo<T, U, T>* tie(const std::string & inName, T U::* inValue)
    {
        MemberInfo<T, U, T>* vb = new MemberInfo<T, U, T>(inName, inValue);
        m_members.push_back(vb);
        return vb;
    }

    template<typename T, class U>
    MemberInfo<T*, U, T>* tie(const std::string & inName, T* U::* inValue)
    {
        MemberInfo<T*, U, T>* vb = new MemberInfoPtr<T*, U, T>(inName, inValue);
        m_members.push_back(vb);
        return vb;
    }
private:
    std::vector<IMember*> m_members;
};

//------------------------------------------------------------
template<typename T>
struct ClassType : public IClassType
{
    static TypeInfo<T> btype;

    ClassType() : IClassType("")
    {
        T::tieMembers(this, m_name);
    }

    virtual const IClassType* getClassType() const { return &btype; }
};

}