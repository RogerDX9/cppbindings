#ifdef _MSC_VER
#include "..\..\build\vs2015\stdafx.h"
#endif

#include "..\Car.h"
#include "..\Wheel.h"

#include <iostream>

#include "..\..\rapidjson\include\rapidjson\writer.h"

using namespace std;

void serializeJSON(const ITypeInfo* inType, const void* inInstance, rapidjson::Writer<rapidjson::StringBuffer>& inWriter)
{
    if (inType->getType() == EBool)
    {
        inWriter.Bool(inType->getValue<bool>(inInstance));
    }
    else if (inType->getType() == EInt)
    {
        inWriter.Int(inType->getValue<int>(inInstance));
    }
    else if (inType->getType() == EUInt)
    {
        inWriter.Uint(inType->getValue<unsigned>(inInstance));
    }
    else if (inType->getType() == EDouble)
    {
        inWriter.Double(inType->getValue<double>(inInstance));
    }
    else if (inType->getType() == EClass)
    {
        const void* instance = inType->getValuePtr(inInstance);

        if (instance != NULL)
        {
            inWriter.StartObject();

            const void*                     classInstance = inInstance;
            const IClassType*               classType = inType->getClassType();
            const std::vector<IMember*>*    members = classType->getMembers();

            for (std::vector<IMember*>::const_iterator it = members->begin(); it != members->end(); ++it)
            {
                const IMember*      m = (*it);
                const void*         memberPtr  = m->getPtr(classInstance);
                const char*         memberName = m->getName();
                const ITypeInfo*    memberType = m->getTypeInfo();

                inWriter.String(memberName);

                serializeJSON(memberType, memberPtr, inWriter);
            }

            inWriter.EndObject();
        }
        else
        {
            inWriter.Null();
        }
    }
    else if (inType->getType() == EArray)
    {
        inWriter.StartArray();

        const void*         arrayInstance = inInstance;
        const IArrayType*   arrayType = inType->getArrayType();

        size_t size = arrayType->getCount(arrayInstance);
        for (size_t idx = 0; idx < size; ++idx)
        {
            const void* valueInstance = arrayType->getValuePtr(arrayInstance, idx);
            serializeJSON(arrayType->getElementType(), valueInstance, inWriter);
        }
        inWriter.EndArray();
    }
    else
    {
        assert(false);
    }
}

template<class T>
void serializeJSON(T& outObject, rapidjson::Writer<rapidjson::StringBuffer>& inWriter)
{
    TypeInfo<T> type;
    serializeJSON(&type, &outObject, inWriter);
}

#ifdef _MSC_VER
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
    Car car;
    car.m_speed = 1;
    car.m_id = 777;
    car.m_mass = 1075;
    car.m_wheelPtr = NULL;
    car.m_wheel.m_rotation = 15;
    car.m_wheel.m_screws.push_back(1);
    car.m_wheel.m_screws.push_back(2);
    car.m_wheel.m_screws.push_back(3);
    car.m_wheel.m_screws.push_back(4);
    car.m_wheels.push_back(car.m_wheel);

    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    serializeJSON(car, writer);
    std::cout << s.GetString() << std::endl;

    return 0;
}