# Project Lyra
Project Lyra provides a self-contained library for obtaining a snapshot of properties of the system a process is running on for the purpose of documenting the environment for reproducability of research. The library is derived from the [TRRojan project](https://github.com/UniStuttgart-VISUS/trrojan), but intended for reuse in other projects without the rest of the project's features.

## Building
The project is built using CMake. Configure and compile it to obtain a DLL that can be used in your project.

## Usage
The interface of the library is mostly built around a class named `property_set`. A property set is a key-value store that can hold different value types that are indexed by strings as keys. The possible types for the values are identified by the `property_type` enumeration. This enumeration holds commonly used types like strings, numbers, but also property sets. The latter allow for hierarchical structuring of information.

Property sets can be obtained by calling the appropriate factory functions. For instance, the following code obtains all raw data we can get:
```cpp
#include "visus/lyra/rawh"

auto data = visus::lyra::raw::get();
```

There are different ways of accessing the data stored in a property set. The most convenient one is probably the `visit` method:
```cpp
data.visit([](const char *name, const auto value, const std::size_t cnt) {
    // Do something with the information. The type of the 'value' is dependent
    // on the type of the property. However, it is always a pointer to 'cnt'
    // consecutive values. Usually, 'cnt' will be 1, but there might be
    // vector-valued properties, too.
});
```
If the callback wants to abort the enumeration of the properties prematurely, it can do so by returning a `bool` that tells the proeprty set when to continue and when not:
```cpp
data.visit([](const char *name, const auto value, const std::size_t cnt) {
    // Abort the enumeration once the first property with more than one value
    // has been found.
    return (cnt > 1);
});
```

The number and names of the properties can be obtained via the `properties` method:
```cpp
// Note that the pointers returned are valid as long as the property set is alive.
std::vector<const char *> names(data.properties());
data.properties(names.data(), names.size());
```

Individual properties can be retrieved via the `get` method:
```cpp
const void *value;
std::size_t cnt;
visus::lyra::property_type type;

if (data.get(value, cnt, type, "CPU")) {
    // Do something with the information. The method will return false if the
    // requested property does not exist. Note that the parameter must be encoded
    // in UTF-8. Other encodings might spuriously fail.
}
```

The library provides descriptors for well-known properties. These have a form like:
```cpp
struct environment final {
    typedef property_set type;
    static constexpr auto name = u8"Environment";
};
```

Such properties can be used to obtain individual properties in a more convenient manner:
```cpp
std::size_t cnt;
auto env = data.get<visus::lyra::raw::environment>(cnt);
```
The method returns a pointer to an array of `cnt` values or `nullptr` if the property does not exist. There is a parameterless overload of the method which just returns the pointer, but no counter. For instance, the first (and only) timestamp can be retrieved like this:
```cpp
auto ts = data.get<visus::lyra::raw::timestamp>();
```

The data from a property set can be easily persisted in the form of JSON strings. Use the `json` method for that:
```cpp
std::string snapshot = data.json();
```
The JSON string will be cached within the property set and remains valid as long as the object exists unless copied.
