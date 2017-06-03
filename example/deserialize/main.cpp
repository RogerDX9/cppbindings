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

#ifdef _MSC_VER
#include "..\..\build\vs2015\stdafx.h"
#endif

#include "..\Car.h"
#include "..\Wheel.h"
#include "..\Tools.h"

#include "..\..\rapidjson\include\rapidjson\document.h"

using namespace std;

void deserializeJSON(const ITypeInfo* inType, const void* inInstance, rapidjson::Value& inValue)
{
	if (inType->getType() == EBool)
	{
		inType->setValue<bool>(inInstance, inValue.GetBool());
	}
	else if (inType->getType() == EInt)
	{
		inType->setValue<int>(inInstance, inValue.GetInt());
	}
	else if (inType->getType() == EUInt)
	{
		inType->setValue<unsigned>(inInstance, inValue.GetUint());
	}
	else if (inType->getType() == EDouble)
	{
		inType->setValue<double>(inInstance, inValue.GetDouble());
	}
	else if (inType->getType() == EClass)
	{
		const IClassType*				classType = inType->getClassType();
		const std::vector<IMember*>*	members   = classType->getMembers();
		for ( std::vector<IMember*>::const_iterator it = members->begin(); it != members->end(); ++it )
		{
			const IMember*		m =				(*it);
			const void*			instance =		m->getValuePtr(inInstance);
			const char*			memberName =	m->getName();
			const ITypeInfo*	memberType =	m->getTypeInfo();

			if (inValue.HasMember(memberName) && !m->isPointer())
			{
				rapidjson::Value& v = inValue[memberName];
				deserializeJSON(m->getTypeInfo(), instance, v);
			}
		}
	}
	else if (inType->getType() == EArray)
	{
		const void* arrayInstance = inInstance;

		size_t size = inValue.Size();
		inType->arrayResize(arrayInstance, size);
		for (size_t idx = 0; idx < size; ++idx)
		{
			rapidjson::Value& v = inValue[idx];
			const void* valueInstance = inType->getArrayValuePtr(arrayInstance, idx);
			deserializeJSON(inType->getElementType(), valueInstance, v); 
		}
	}
}

template<class T>
void deserializeJSON(T& outObject, rapidjson::Value& inValue)
{
	deserializeJSON(&TypeInfo<T>::btype, &outObject, inValue);
}

#ifdef _MSC_VER
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
	Car car; // Test
	rapidjson::Document document;

	const char * jsonStr = 
		"{\"m_automaticTransmission\":true,\
		\"m_speed\":20,\
		\"m_id\":666,\
		\"m_mass\":1275.0,\
		\"m_wheelPtr\":null,\
		\"m_wheel\":\
			{\"m_rotation\":2,\"m_screws\":[1,2,3,4]},\
		\"m_wheels\":[\
			{\"m_rotation\":3,\"m_screws\":[1,2]},\
			{\"m_rotation\":4,\"m_screws\":[1,2,3,4,4,4,6,7]}\
			]}";

	document.Parse(jsonStr);

	deserializeJSON(car, document);

	printHierarchy(car);

	return 0;
}