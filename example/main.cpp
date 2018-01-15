#include <iostream>

#include "Car.h"
#include "Wheel.h"
#include "Tools.h"

#include "..\rapidjson\include\rapidjson\document.h"
#include "..\rapidjson\include\rapidjson\writer.h"

using namespace std;

//------------------------------------------------------------
void serializeJSON(const CBind::ITypeInfo* inType, const void* inInstance, rapidjson::Writer<rapidjson::StringBuffer>& inWriter)
{
    switch (inType->getType())
    {
    case CBind::EBool:
    {
        inWriter.Bool(inType->getValue<bool>(inInstance));
        break;
    }
    case CBind::EInt:
    {
        inWriter.Int(inType->getValue<int>(inInstance));
        break;
    }
    case CBind::EUInt:
    {
        inWriter.Uint(inType->getValue<unsigned>(inInstance));
        break;
    }
    case CBind::EDouble:
    {
        inWriter.Double(inType->getValue<double>(inInstance));
        break;
    }
    case CBind::EClass:
    {
        const void* instance = inType->getValuePtr(inInstance);

        if (instance != NULL)
        {
            inWriter.StartObject();

            const void*                             classInstance = inInstance;
            const CBind::IClassType*                classType = inType->getClassType();
            const std::vector<CBind::IMember*>*     members = classType->getMembers();

            for (std::vector<CBind::IMember*>::const_iterator it = members->begin(); it != members->end(); ++it)
            {
                const CBind::IMember*   m = (*it);
                const void*             memberPtr = m->getPtr(classInstance);
                const char*             memberName = m->getName();
                const CBind::ITypeInfo* memberType = m->getTypeInfo();

                inWriter.String(memberName);

                serializeJSON(memberType, memberPtr, inWriter);
            }

            inWriter.EndObject();
        }
        else
        {
            inWriter.Null();
        }
        break;
    }
    case CBind::EArray:
    {
        inWriter.StartArray();

        const void*                 arrayInstance = inInstance;
        const CBind::IArrayType*    arrayType = inType->getArrayType();

        size_t size = arrayType->getCount(arrayInstance);
        for (size_t idx = 0; idx < size; ++idx)
        {
            const void* valueInstance = arrayType->getValuePtr(arrayInstance, idx);
            serializeJSON(arrayType->getElementType(), valueInstance, inWriter);
        }
        inWriter.EndArray();
        break;
    }
    default:
        assert(false);
        break;
    }
}

//------------------------------------------------------------
template<class T>
void serializeJSON(T& inInstance, rapidjson::Writer<rapidjson::StringBuffer>& inWriter)
{
    serializeJSON(&CBind::TypeInfo<T>::btype, &inInstance, inWriter);
}

//------------------------------------------------------------
void deserializeJSON(const CBind::ITypeInfo* inType, const void* outInstance, rapidjson::Value& inValue)
{
    switch (inType->getType())
    {
    case CBind::EBool:
    {
        inType->setValue<bool>(outInstance, inValue.GetBool());
        break;
    }
    case CBind::EInt:
    {
        inType->setValue<int>(outInstance, inValue.GetInt());
        break;
    }
    case CBind::EUInt:
    {
        inType->setValue<unsigned>(outInstance, inValue.GetUint());
        break;
    }
    case CBind::EDouble:
    {
        inType->setValue<double>(outInstance, inValue.GetDouble());
        break;
    }
    case CBind::EClass:
    {
        const void*                         classInstance = outInstance;
        const CBind::IClassType*            classType = inType->getClassType();
        const std::vector<CBind::IMember*>* members = classType->getMembers();

        for (std::vector<CBind::IMember*>::const_iterator it = members->begin(); it != members->end(); ++it)
        {
            const CBind::IMember*   m = (*it);
            const void*             memberPtr = m->getPtr(classInstance);
            const char*             memberName = m->getName();
            const CBind::ITypeInfo* memberType = m->getTypeInfo();

            if (inValue.HasMember(memberName) && !m->isPointer())
            {
                deserializeJSON(m->getTypeInfo(), memberPtr, inValue[memberName]);
            }
        }
        break;
    }
    case CBind::EArray:
    {
        const void*                 arrayInstance = outInstance;
        const CBind::IArrayType*    arrayType = inType->getArrayType();
        size_t                      size = inValue.Size();

        arrayType->resize(arrayInstance, size);

        for (size_t idx = 0; idx < size; ++idx)
        {
            const void* valuePtr = arrayType->getValuePtr(arrayInstance, idx);
            deserializeJSON(arrayType->getElementType(), valuePtr, inValue[idx]);
        }
        break;
    }
    default:
        assert(false);
        break;
    }
}

//------------------------------------------------------------
template<class T>
void deserializeJSON(T& outInstance, rapidjson::Value& inValue)
{
    deserializeJSON(&CBind::TypeInfo<T>::btype, &outInstance, inValue);
}

//------------------------------------------------------------
int main()
{
    std::cout << "Seserialize:" << std::endl;
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
    }

    std::cout << std::endl << "Deserialize:" << std::endl;
    {
        Car car;
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
        printBindings(car);
    }

    return 0;
}