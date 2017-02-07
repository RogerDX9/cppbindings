/*Copyright (C) 2014 Dmytro Shchukin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

#ifndef REFLECTION_H
#define REFLECTION_H

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
	EArray,
	EPointer
};

//------------------------------------------------------------
struct IMember;

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

	// Class
	virtual bool isMember() const { return false; }
	virtual const char* getMemberName() const { return NULL; }
	virtual const char* getName() const { return m_name.c_str(); }
	virtual const std::vector<ITypeInfo*>* getMembers() const { return NULL; }

	// Array
	virtual const ITypeInfo* getElementType() const { return NULL; }
	virtual const size_t getArrayCount(const void* instance) const { return 0; }
	virtual const void* getArrayValuePtr(const void* instance, int inIndex) const { return NULL; }
	virtual void arrayResize(const void* instance, size_t n) const {}

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
// main tamplate
template<typename T>
struct TypeInfo : public ITypeInfo
{
	static TypeInfo<T> btype;
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
};

//------------------------------------------------------------
// template specialization for bool
template<>
struct TypeInfo <bool>: public PrimitiveType<bool>
{
	TypeInfo(): PrimitiveType<bool>("bool") {}

	virtual EType getType() const
	{
		return EBool;
	}
};

//------------------------------------------------------------
// template specialization for int
template<>
struct TypeInfo <int>: public PrimitiveType<int>
{
	TypeInfo(): PrimitiveType<int>("int") {}

	virtual EType getType() const
	{
		return EInt;
	}
};

//------------------------------------------------------------
// template specialization for unsigned
template<>
struct TypeInfo <unsigned>: public PrimitiveType<unsigned>
{
	TypeInfo(): PrimitiveType<unsigned>("unsigned") {}

	virtual EType getType() const
	{
		return EUInt;
	}
};

//------------------------------------------------------------
// template specialization for float
template<>
struct TypeInfo <float>: public PrimitiveType<float>
{
	TypeInfo(): PrimitiveType<float>("float") {}

	virtual EType getType() const
	{
		return EFloat;
	}
};

//------------------------------------------------------------
// template specialization for double
template<>
struct TypeInfo <double>: public PrimitiveType<double>
{
	TypeInfo(): PrimitiveType<double>("double") {}

	virtual EType getType() const
	{
		return EDouble;
	}
};

template<typename T>
TypeInfo<T> PrimitiveType<T>::btype;

//------------------------------------------------------------
// base class for arrays
template<typename T>
struct ArrayType: public ITypeInfo
{
	static TypeInfo<T> btype;

	ArrayType(const std::string & inName)
		: ITypeInfo(inName)
	{
	}

	virtual EType getType() const
	{
		return EArray;
	}
};

template<typename TElem>
struct ArrayTypeBase : public ITypeInfo
{
	ArrayTypeBase(const std::string & inName)
		: ITypeInfo(inName)
		, m_elementType(&TypeInfo<TElem>::btype)
	{
	}

	virtual EType getType() const
	{
		return EArray;
	}

	virtual const ITypeInfo* getElementType() const 
	{ 
		return m_elementType;
	}
private:
	const ITypeInfo* m_elementType;
};

template<typename TElem>
struct STDVectorType : public ArrayTypeBase<TElem>
{
	STDVectorType(const std::string & inName)
		: ArrayTypeBase<TElem>(inName)
	{
	}

	static TypeInfo<std::vector<TElem>> btype;

	virtual const size_t getArrayCount(const void* instance) const
	{
		const std::vector<TElem>* array = (const std::vector<TElem>*)instance;
		return array->size();
	}

	virtual const void* getArrayValuePtr(const void* instance, int inIndex) const
	{
		const std::vector<TElem>* array = (const std::vector<TElem>*)instance;
		const void* value = &array->at(inIndex);
		return value;
	}
	virtual void arrayResize(const void* instance, size_t n) const
	{
		std::vector<TElem>* array = (std::vector<TElem>*)instance;
		array->resize(n);
	}
};

//------------------------------------------------------------
// template specialization for std::vector
template<typename TElem>
struct TypeInfo < std::vector<TElem> >: public STDVectorType<TElem>
{
	TypeInfo(): STDVectorType< TElem >("std::vector") {}
};

//template<>
//TypeInfo<std::vector<int>> ArrayType<std::vector<int>>::btype;

//------------------------------------------------------------
// template specialization for std::vector<int>
//template<>
//struct TypeInfo < std::vector<int> > : public ArrayType< std::vector<int> >
//{
//	TypeInfo() : ArrayType< std::vector<int> >("std::vector<int>") {}
//
//	virtual const ITypeInfo* getElementType() const { return &TypeInfo<int>::btype; }
//
//	virtual const size_t getArrayCount(const void* instance) const
//	{
//		const std::vector<int>* array = (const std::vector<int>*)instance;
//		return array->size();
//	}
//	virtual const void* getArrayValuePtr(const void* instance, int inIndex) const
//	{
//		const std::vector<int>* array = (const std::vector<int>*)instance;
//		const void* value = &array->at(inIndex);
//		return value;
//	}
//	virtual void arrayResize(const void* instance, size_t n) const
//	{
//		std::vector<int>* array = (std::vector<int>*)instance;
//		array->resize(n);
//	}
//};

//------------------------------------------------------------
// base class for pointers
//template<class T*>
//struct PointerType : public ITypeInfo
//{
//	static TypeInfo<T> btype;
//
//	PointerType()
//		: ITypeInfo("pointer")
//	{
//	}
//
//	virtual EType getType() const
//	{
//		return EPointer;
//	}
//};
//
//template<typename T>
//TypeInfo<T> PointerType<T>::btype;

//------------------------------------------------------------
struct IMember : public ITypeInfo
{
	IMember(const ITypeInfo* inType) 
		: ITypeInfo(inType->getName())
		, m_type(inType) 
	{}

	virtual bool isPointer() const { return m_type->isPointer(); }
	virtual EType getType() const { return m_type->getType(); }
	virtual const std::vector<ITypeInfo*>* getMembers() const { return m_type->getMembers(); }

	// Array
	virtual const ITypeInfo* getElementType() const { return m_type->getElementType(); }
	virtual const size_t getArrayCount(const void* instance) const { return m_type->getArrayCount(instance); }
	virtual const void* getArrayValuePtr(const void* instance, int inIndex) const { return m_type->getArrayValuePtr(instance, inIndex); }
	virtual void arrayResize(const void* instance, size_t n) const { m_type->arrayResize(instance, n); }

private:
	const ITypeInfo* m_type;
};

//------------------------------------------------------------
// TType is the same T, it's done to differentiate pointer from regular value
template<typename T, class U, typename TType>
struct MemberInfo: public IMember
{
protected:
	std::string		m_memberName;
	T U::*			m_pMember;

public:

	MemberInfo(const std::string& inName, T U::* pMember)
		: IMember(&TypeInfo<TType>::btype)
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
		U * iPtr = (U*)instance;
		iPtr->*m_pMember = *vPtr;
	}
};

//------------------------------------------------------------
template<typename T, class U, typename TType>
struct MemberPointerInfo: public MemberInfo<T, U, TType>
{
	MemberPointerInfo(const std::string& inName, T U::* pMember)
		: MemberInfo<T, U, TType>(inName, pMember)
	{
	}

	virtual bool isPointer() const { return true; }

	virtual const void* getValuePtr(const void* instance) const
	{
		U * iPtr = (U*)instance;
		T U::* mPtr = this->m_pMember;
		return iPtr->*mPtr;
	}
};

//------------------------------------------------------------
// base for all class template specialization
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
	MemberInfo<T, U, T>* bind(const std::string & inName, T U::* inValue)
	{
		MemberInfo<T, U, T>* vb = new MemberInfo<T, U, T>(inName, inValue);
		m_members.push_back(vb);
		return vb;
	}

	template<typename T, class U>
	MemberInfo<T*, U, T>* bind(const std::string & inName, T* U::* inValue)
	{
		MemberInfo<T*, U, T>* vb = new MemberPointerInfo<T*, U, T>(inName, inValue);
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
	static TypeInfo<T> btype;

	ClassType()	: IClassType("")
	{
		m_name = T::bindReflection(this);
	}
};

template<typename T>
TypeInfo<T> ClassType<T>::btype;

template<>
TypeInfo<std::vector<int>> STDVectorType<int>::btype;

//template<typename T>
//TypeInfo<std::vector<T>> STDVectorTypeBase<T>::btype;



//template<typename T>
//TypeInfo<T> ArrayType<T>::btype;

//template<typename T>
//TypeInfo<T> PrimitiveType<T>::btype;

#endif