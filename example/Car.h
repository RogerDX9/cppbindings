#pragma once 

#include "..\cppbindings\reflection.h"
#include "Wheel.h"
#include <vector>

struct Car
{
    bool                m_automaticTransmission;
    int                 m_speed;
    unsigned            m_id;
    double              m_mass;
    Wheel*              m_wheelPtr;
    Wheel               m_wheel;
    std::vector<Wheel>  m_wheels;

    static void tieReflection(IClassType * inReflection, std::string & outName)
    {
        inReflection->tie("m_automaticTransmission",   &Car::m_automaticTransmission);
        inReflection->tie("m_speed",                   &Car::m_speed);
        inReflection->tie("m_id",                      &Car::m_id);
        inReflection->tie("m_mass",                    &Car::m_mass);
        inReflection->tie("m_wheelPtr",                &Car::m_wheelPtr);
        inReflection->tie("m_wheel",                   &Car::m_wheel);
        inReflection->tie("m_wheels",                  &Car::m_wheels);

        outName = "Car";
    }
};

// template specification
template<> struct TypeInfo <Car>: public ClassType<Car> {};