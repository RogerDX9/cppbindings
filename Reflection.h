#ifndef REFLECTION_H
#define REFLECTION_H

#include <string>
#include <map>
#include <assert.h>
#include <type_traits>
#include <vector>

enum EType
{
	NotImplemented,
	Float,
	Int,
	String,
	Class,
	Array,
};

//------------------------------------------------------------
struct IType
{
	std::string m_name;

	IType(const std::string & inName)
		: m_name(inName)
	{
	}

	virtual EType getType() { return NotImplemented; }
	virtual bool isPointer() { return false; }
	virtual IType* getElementType() { return NULL; }
	virtual IType* getMember(const std::string & memberName) { return NULL; }
};

//------------------------------------------------------------
template<typename T>
struct Type : public IType
{
};

//------------------------------------------------------------
template<typename T>
struct IntType: public IType
{
	IntType(const std::string & inName)
		: IType(inName)
	{
	}

	virtual EType getType()
	{
		return Int;
	}
};

//------------------------------------------------------------
template<typename T>
struct ArrayType: public IType
{
	ArrayType(const std::string & inName)
		: IType(inName)
	{
	}

	virtual EType getType()
	{
		return Array;
	}
};

//------------------------------------------------------------
template<typename T>
struct Type <std::vector<T>>: public ArrayType<std::vector<T>>
{
	Type<T> m_elementType;
	Type(): ArrayType<std::vector<T>>("std::vector") {}

	virtual IType* getElementType() { return &m_elementType; }
};

//------------------------------------------------------------
template<>
struct Type <int>: public IntType<int>
{
	Type(): IntType<int>("int") {}
};

//------------------------------------------------------------
template<typename T, class U, typename TType>
struct MemberBinding: public Type<TType>
{
	T U::*			m_pMember;

	void setMember(T U::* pMember)
	{
		m_pMember = pMember;
	}

public:

	void setValue(U & instance, const T & value)
	{
		instance.*m_pMember = value;
	}

	const T& getValue(const U & instance)
	{
		return instance.*m_pMember;
	}

	void default(const T& inValue)
	{
		*m_pMember = inValue;
	}
};

//------------------------------------------------------------
template<typename T, class U, typename TType>
struct MemberPointerBinding: public MemberBinding<T, U, TType>
{
	virtual bool isPointer() { return true; }
};


//------------------------------------------------------------
struct IClassType: public IType
{
private:
	std::map<std::string, IType*> m_members;

public:
	IClassType(const std::string & inName)
		: IType(inName)
	{
	}

	template<typename T, class U>
	MemberBinding<T, U, T>* bind(const std::string & inName, T U::* inValue)
	{
		MemberBinding<T, U, T>* vb = new MemberBinding<T, U, T>();
		vb->setMember(inValue);
		m_members[inName] = vb;
		return vb;
	}

	template<typename T, class U>
	MemberBinding<T*, U, T>* bind(const std::string & inName, T* U::* inValue)
	{
		MemberBinding<T*, U, T>* vb = new MemberPointerBinding<T*, U, T>();
		vb->setMember(inValue);
		m_members[inName] = vb;
		return vb;
	}

	virtual IType* getMember(const std::string & memberName)
	{
		assert(m_members.find(memberName) != m_members.end());
		return m_members[memberName];
	}

	template<typename T, class U>
	void setValue(const std::string & memberName, U & instance, const T & inValue)
	{
		MemberBinding<T, U, T>* vb = static_cast<MemberBinding<T, U, T>*>(getMember(memberName));
		vb->setValue(instance, inValue);
	}
};

//------------------------------------------------------------
template<typename T>
struct ClassType: public IClassType
{
	ClassType(const std::string & inName)
		: IClassType(inName)
	{
		T::bindReflection(this);
	}
	virtual EType getType()
	{
		return Class;
	}
};

#endif