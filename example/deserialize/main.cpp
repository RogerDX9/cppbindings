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
        const void*                     classInstance = inInstance;
        const IClassType*               classType = inType->getClassType();
        const std::vector<IMember*>*    members   = classType->getMembers();

        for ( std::vector<IMember*>::const_iterator it = members->begin(); it != members->end(); ++it )
        {
            const IMember*      m =             (*it);
            const void*         memberPtr  =    m->getPtr(classInstance);
            const char*         memberName =    m->getName();
            const ITypeInfo*    memberType =    m->getTypeInfo();

            if (inValue.HasMember(memberName) && !m->isPointer())
            {
                deserializeJSON(m->getTypeInfo(), memberPtr, inValue[memberName]);
            }
        }
    }
    else if (inType->getType() == EArray)
    {
        const void*         arrayInstance   = inInstance;
        const IArrayType*   arrayType       = inType->getArrayType();
        size_t              size            = inValue.Size();
        
        arrayType->resize(arrayInstance, size);

        for (size_t idx = 0; idx < size; ++idx)
        {
            const void* valuePtr = arrayType->getValuePtr(arrayInstance, idx);
            deserializeJSON(arrayType->getElementType(), valuePtr, inValue[idx]);
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

    printBindings(car);

    return 0;
}