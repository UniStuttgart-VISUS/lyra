# Project Lyra
Project Lyra provides a self-contained library for obtaining a snapshot of properties of the system a process is running on for the purpose of documenting the environment for reproducability of research. The library is derived from the [TRRojan project](https://github.com/UniStuttgart-VISUS/trrojan), but intended for reuse in other projects without the rest of the project's features.

## Building
The project is built using CMake. Configure and compile it to obtain a DLL that can be used in your project.

## Usage
The interface of the library is mostly built around a class named `property_set`. A property set is a key-value store that can hold different value types that are indexed by strings as keys. The possible types for the values are identified by the `property_type` enumeration. This enumeration holds commonly used types like strings, numbers, but also property sets. The latter allow for hierarchical structuring of information.

Property sets can be obtained by calling the appropriate factory functions. For instance, the following code obtains data stored in the system's SMBIOS:
```cpp
#include "visus/lyra/smbios.h"

auto smbios = visus::lyra::smbios::get_smbios();
```

There are different ways of accessing the data stored in a property set. The most convenient one is probably the `visit` method:
```cpp
smbios.visit([](const std::string& name, const auto value, const std::size_t cnt) {
    // Do something with the information. The type of the 'value' is dependent
    // on the type of the property. However, it is always a pointer to 'cnt'
    // consecutive values. Usually, 'cnt' will be 1, but there might be
    // vector-valued properties, too.
});
```

The data from a property set can be easily persisted in the form of JSON strings. Use the `json` method for that:
```cpp
std::string smbios_string = smbios.json();
```