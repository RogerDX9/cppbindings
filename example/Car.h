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

    static void tieMembers(CBind::IClassType * classType, std::string & outName)
    {
        classType->tie("m_automaticTransmission",   &Car::m_automaticTransmission);
        classType->tie("m_speed",                   &Car::m_speed);
        classType->tie("m_id",                      &Car::m_id);
        classType->tie("m_mass",                    &Car::m_mass);
        classType->tie("m_wheelPtr",                &Car::m_wheelPtr);
        classType->tie("m_wheel",                   &Car::m_wheel);
        classType->tie("m_wheels",                  &Car::m_wheels);

        outName = "Car";
    }
};

namespace CBind
{
    // template specification
    template<> struct TypeInfo <Car> : public ClassType<Car> {};
}