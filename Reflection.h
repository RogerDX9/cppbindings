#ifndef REFLECTION_H
#define REFLECTION_H

#include <string>
#include <map>
#include <assert.h>
#include <vector>

enum EType
{
	ENotImplemented,
	EFloat,
	EInt,
	EString,
	EClass,
	EArray,
};

//------------------------------------------------------------
struct ITypeInfo
{
protected:
	std::string m_name;
public:
	ITypeInfo(const std::string & inName)
		: m_name(inName)
	{
	}

	virtual ~ITypeInfo() {}

	virtual EType getType() const { return ENotImplemented; }
	virtual bool isPointer() const { return false; }

	virtual bool isMember() const { return false; }
	virtual const char* getMemberName() const { return NULL; }
	virtual const char* getName() const { return m_name.c_str(); }
	virtual const std::vector<ITypeInfo*>* getMembers() const { return NULL; }

	virtual const ITypeInfo* getElementType() const { return NULL; }
	virtual const size_t getArrayCount(const void* instance) const { return 0; }
	virtual const void* getArrayValuePtr(const void* instance, int inIndex) const { return NULL; }

	virtual const void* getValuePtr(const void* instance) const { return instance; }
	virtual void setValuePtr(const void* instance, const void* value) const { assert(true); }

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
};

//------------------------------------------------------------
template<typename T>
struct TypeInfo : public ITypeInfo
{
};

//------------------------------------------------------------
template<typename T>
struct IntType: public ITypeInfo
{
	IntType(const std::string & inName)
		: ITypeInfo(inName)
	{
	}

	virtual EType getType() const
	{
		return EInt;
	}

	virtual void setValuePtr(const void* instance, const void* value) const
	{
		int * vPtr = (int*)value;
		int * iPtr = (int*)instance;
		*iPtr = *vPtr;
	}
};

//------------------------------------------------------------
template<>
struct TypeInfo <int>: public IntType<int>
{
	TypeInfo(): IntType<int>("int") {}
};

//------------------------------------------------------------
template<typename T>
struct ArrayType: public ITypeInfo
{
	ArrayType(const std::string & inName)
		: ITypeInfo(inName)
	{
	}

	virtual EType getType() const
	{
		return EArray;
	}
};

//------------------------------------------------------------
// template specialization for std::vector
template<typename T>
struct TypeInfo <std::vector<T>>: public ArrayType<std::vector<T>>
{
private:
	TypeInfo<T> m_elementType;
public:
	TypeInfo(): ArrayType<std::vector<T>>("std::vector") {}

	virtual const ITypeInfo* getElementType() const { return &m_elementType; }

	virtual const size_t getArrayCount(const void* instance) const 
	{
		const std::vector<T>* array = (const std::vector<T>*)instance;
		return array->size();
	}
	virtual const void* getArrayValuePtr(const void* instance, int inIndex) const
	{
		const std::vector<T>* array = (const std::vector<T>*)instance;
		const void* value = &array->at(inIndex);
		return value;
	}
};

//------------------------------------------------------------
template<typename T, class U, typename TType>
struct MemberBinding: public TypeInfo<TType>
{
protected:
	std::string		m_memberName;
	T U::*			m_pMember;

public:

	MemberBinding(const std::string& inName, T U::* pMember)
		: TypeInfo<TType>()
		, m_memberName(inName)
		, m_pMember(pMember)
	{
	}

	virtual bool isMember() const { return true; }

	virtual const char* getMemberName() const { return m_memberName.c_str(); }

	virtual const void* getValuePtr(const void* instance) const
	{
		U * iPtr = (U*)instance;
		return &(iPtr->*m_pMember);
	}

	virtual void setValuePtr(const void* instance, const void* value) const
	{
		T * vPtr = (T*)value;
		((U*)instance)->*m_pMember = *vPtr;
	}
};

//------------------------------------------------------------
template<typename T, class U, typename TType>
struct MemberPointerBinding: public MemberBinding<T, U, TType>
{
	MemberPointerBinding(const std::string& inName, T U::* pMember)
		: MemberBinding<T, U, TType>(inName, pMember)
	{
	}

	virtual bool isPointer() const { return true; }

	virtual const void* getValuePtr(const void* instance) const
	{
		return ((U*)instance)->*m_pMember;
	}
};


//------------------------------------------------------------
struct IClassType: public ITypeInfo
{
private:
	std::vector<ITypeInfo*> m_members;

public:
	IClassType(const std::string & inName)
		: ITypeInfo(inName)
	{
	}

	~IClassType()
	{
		for (std::vector<ITypeInfo*>::const_iterator it = m_members.begin(); it != m_members.end(); ++it)
		{
			delete (*it);
		}
	}

	template<typename T, class U>
	MemberBinding<T, U, T>* bind(const std::string & inName, T U::* inValue)
	{
		MemberBinding<T, U, T>* vb = new MemberBinding<T, U, T>(inName, inValue);
		m_members.push_back(vb);
		return vb;
	}

	template<typename T, class U>
	MemberBinding<T*, U, T>* bind(const std::string & inName, T* U::* inValue)
	{
		MemberBinding<T*, U, T>* vb = new MemberPointerBinding<T*, U, T>(inName, inValue);
		m_members.push_back(vb);
		return vb;
	}

	virtual EType getType() const
	{
		return EClass;
	}

	virtual const std::vector<ITypeInfo*>* getMembers() const { return &m_members; }
};

//------------------------------------------------------------
template<typename T>
struct ClassType: public IClassType
{
	ClassType()	: IClassType("")
	{
		m_name = T::bindReflection(this);
	}
};

#endif