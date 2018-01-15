#pragma once 

#include "..\cppbindings\Reflection.h"

struct Wheel
{
    int                 m_rotation;
    std::vector<int>    m_screws;

    static void tieMembers(CBind::IClassType * classType, std::string outName)
    {
        classType->tie("m_rotation",    &Wheel::m_rotation);
        classType->tie("m_screws",      &Wheel::m_screws);

        outName = "Wheel";
    }
};

namespace CBind
{
    // template specification
    template<> struct TypeInfo <Wheel> : public ClassType<Wheel> {};
}
