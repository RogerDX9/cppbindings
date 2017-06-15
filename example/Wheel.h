#pragma once 

#include "..\cppbindings\reflection.h"

struct Wheel
{
    int                 m_rotation;
    std::vector<int>    m_screws;

    static void tieReflection(ClassType<Wheel> * inReflection, std::string outName)
    {
        inReflection->tie("m_rotation",    &Wheel::m_rotation);
        inReflection->tie("m_screws",      &Wheel::m_screws);

        outName = "Wheel";
    }
};

// template specification
template<> struct TypeInfo <Wheel>: public ClassType<Wheel> {};
