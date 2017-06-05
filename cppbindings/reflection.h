#pragma once

#include <string>
#include <assert.h>
#include <vector>

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

    virtual const void*         getValuePtr(const void* instance) const { assert(false); return instance; }
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
// main tamplate
template<typename T>
struct TypeInfo : public ITypeInfo
{
    //static TypeInfo<T> btype;
};

//------------------------------------------------------------
// base class for build-in types
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

    virtual const void* getValuePtr(const void* instance) const 
    { 
        return instance;
    }
};

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
// base for containers
struct IArrayType : public ITypeInfo
{
    IArrayType(const std::string & inName)
        : ITypeInfo(inName)
    {
    }

    virtual EType               getType() const { return EArray; }

    virtual const ITypeInfo*    getElementType() const = 0;
    virtual const size_t        getArrayCount(const void* instance) const = 0;
    virtual const void*         getArrayValuePtr(const void* instance, int inIndex) const = 0;
    virtual void                arrayResize(const void* instance, size_t n) const = 0;
};

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
        return &TypeInfo<TContainer::value_type>::btype;
    }

    virtual const size_t getArrayCount(const void* instance) const
    {
        const TContainer* array = (const TContainer*)instance;
        return array->size();
    }

    virtual const void* getArrayValuePtr(const void* instance, int inIndex) const
    {
        const TContainer* array = (const TContainer*)instance;
        const void* value = &array->at(inIndex);
        return value;
    }

    virtual void arrayResize(const void* instance, size_t n) const
    {
        TContainer* array = (TContainer*)instance;
        array->resize(n);
    }
};

//------------------------------------------------------------
// template specialization for std::vector
template<typename TElem>
struct TypeInfo<std::vector<TElem>> : public StdVectorType<std::vector<TElem>>
{
    TypeInfo(): StdVectorType<std::vector<TElem>>("std::vector")
    {
    }
};

//------------------------------------------------------------
struct IMember
{
    IMember(const std::string& inName)
        : m_name(inName)
    {}

    virtual const ITypeInfo*    getTypeInfo() const = 0;

    const char*                 getName() const             { return m_name.c_str(); }
    virtual bool                isPointer() const           { return false; }

    virtual const void*         getValuePtr(const void* instance) const = 0;
    virtual void                setValuePtr(const void* instance, const void* value) const = 0;

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

    virtual const void* getValuePtr(const void* inClassInstance) const
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

    virtual const void* getValuePtr(const void* inClassInstance) const
    {
        U * iPtr = (U*) inClassInstance;
        T U::* mPtr = this->m_pMember;
        return iPtr->*mPtr;
    }

    virtual void setValuePtr(const void* inClassInstance, const void* value) const
    {
        assert(false);
    }
};

//------------------------------------------------------------
// base for all class template specializations
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

    virtual const std::vector<IMember*>* getMembers() const { return &m_members; }

    template<typename T, class U>
    MemberInfo<T, U, T>* bind(const std::string & inName, T U::* inValue)
    {
        MemberInfo<T, U, T>* vb = new MemberInfo<T, U, T>(inName, inValue);
        m_members.push_back(vb);
        return vb;
    }

    template<typename T, class U>
    MemberInfo<T*, U, T>* bind(const std::string & inName, T* U::* inValue)
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
struct ClassType: public IClassType
{
    static TypeInfo<T> btype;

    ClassType() : IClassType("")
    {
        m_name = T::bindReflection(this);
    }

    virtual const IClassType* getClassType() const { return &btype; }
};

//------------------------------------------------------------
template<typename T>
TypeInfo<T> PrimitiveType<T>::btype;

template<typename T>
TypeInfo<T> StdVectorType<T>::btype;

template<typename T>
TypeInfo<T> ClassType<T>::btype;