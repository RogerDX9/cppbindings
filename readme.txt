CppBindings

Copyright (C) 2014 Dmytro Shchukin(dmytro.shchukin@gmail.com)

Introduction:
CppBindings allows to add reflection metadata for C++ to your project.

 - Easy to integrate(one function and template specification per class, all can be done in header)
 - No macros used

Compatibility:
CppBindings is cross-platform. Some platform/compiler combinations which have been tested are shown as follows.

 - Visual C++ 2012 on Windows
 - GNU on Cygwin

Installation:
CppBindings is a header-only C++ library. Just copy the /cppbindings folder to system or project's include path.

To build the tests and examples:

On Windows, build the solution at /build/vs2012
On GNU, g++ example/*name*/main.cpp