# CPP Bindings

Template based reflection for C++

## Getting Started

A function with all members described and template specification are required per class:

```
struct Wheel
{
    int                 m_rotation;
    std::vector<int>    m_screws;

    static void tieMembers(CBind::IClassType * classType, std::string outName) // a function
    {
        classType->tie("m_rotation",    &Wheel::m_rotation);
        classType->tie("m_screws",      &Wheel::m_screws);

        outName = "Wheel";
    }
};

namespace CBind
{
    template<> struct TypeInfo <Wheel> : public ClassType<Wheel> {}; // template specification
}
```

Defined types described here:
     https://github.com/spoiltos/cppbindings/blob/master/cppbindings/Reflection.h
     
## Compiled under:
    - visual studio 2015
    - gcc 6.4.0