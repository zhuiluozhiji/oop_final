# Object Serialization Project

This project implements object serialization and deserialization in C++. It includes two main modules: binary serialization and XML serialization.

## Modules

### Binary Serialization
- Located in `include/binary_serialization.h`
- Implements functions for binary serialization and deserialization.
- Supports serialization of:
  - Basic data types (e.g., int, float, double)
  - C++ string type
  - STL containers (e.g., std::pair, std::vector, std::list, std::set, std::map)
- Provides a template mechanism for user-defined types.

### XML Serialization
- Located in `include/xml_serialization.h`
- Implements functions for XML serialization and deserialization using the tinyxml2 library.
- Supports serialization of:
  - Basic data types
  - C++ string type
  - STL containers
- Also supports serialization of user-defined types.

## Testing
- The testing code is located in `test/test_serialization.cpp`.
- It verifies the functionality of both binary and XML serialization modules.
- Tests include serialization and deserialization of basic data types, STL containers, and user-defined types.

## Build Instructions
To build the project, you can use CMake. Follow these steps:

1. Navigate to the project directory.
2. Create a build directory:
   ```
   mkdir build
   cd build
   ```
3. Run CMake to configure the project:
   ```
   cmake ..
   ```
4. Build the project:
   ```
   make
   ```

## Usage
After building the project, you can use the serialization functions provided in the headers to serialize and deserialize your objects. Refer to the header files for detailed function signatures and usage examples.

## Dependencies
- tinyxml2 library for XML serialization.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.