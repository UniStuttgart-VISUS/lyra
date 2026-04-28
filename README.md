# Project Lyra
Project Lyra provides a self-contained library for obtaining a snapshot of the system a process is running on for the purpose of documenting the environment for reproducability of research.

## Building
The project is built using CMake. Configure and compile it to obtain a DLL that can be used in your project.

## Usage
The interface of the library is mostly built around a class named `property_set`. A property set is a key-value store that can hold different value types that are indexed by strings as keys. The possible types for the values are identified by the `property_type` enumeration. This enumeration holds commonly used types like strings, numbers, but also property sets. The latter allow for hierarchical structuring of information.
