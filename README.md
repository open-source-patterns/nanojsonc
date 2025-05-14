## Sponsor this project

**Open Source Patterns Inc** is a 501(c)(3) non-profit organization dedicated to fostering innovation through open source projects.

As a **tax-exempt entity**, your donations to Open Source Patterns are eligible for **tax deductions** and **write-offs**, providing you with an opportunity to support our mission while also receiving a valuable tax benefit.

Your generous contributions enable us to add, maintain and enhance our open source projects, empowering developers and communities worldwide. By donating, you become an integral part of our commitment to advancing technology and promoting accessibility.

## nanoJSONc - Event-Driven JSON Parser for C

[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![CMake on multiple platforms](https://github.com/open-source-patterns/nanojsonc/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/open-source-patterns/nanojsonc/actions/workflows/cmake-multi-platform.yml)

nanoJSONc is a C-based event-driven architecture library for JSON parsing,
making it easy to integrate with your project's logic.

Users can register callback functions to respond to specific events, such as
key-value pairs within JSON objects or indices-values within JSON arrays, offering a
highly customizable and efficient approach to JSON parsing. nanoJSONc is ideal
for applications where tailored event handling is crucial to processing JSON
data effectively.

## Features
- Event-Driven Architecture
- Lightweight - ~250 lines of code
- Robust—almost "crash proof"
- No Dependencies
- Memory-Efficient - No `malloc(3)`
- Simple API, Pure Functions, No side-effects
- Parses deeply nested & mixed JSON objects and arrays (See [examples](https://github.com/open-source-patterns/nanojsonc/tree/main/example))

## Parsing JSON Object

```c
#include <nanojsonc/parser.h>

struct Person {
    char *name;
    int age;
};

static void callback(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    struct Person *person = object;
    if (strcmp(key, "name") == 0) (*person).name = strdup(value);
    if (strcmp(key, "age") == 0) (*person).age = atoi(value);
}

int main(void) {
    char *json = "{\"name\": \"John Doe\", \"age\": 25}";

    struct Person person = (struct Person){0};
    nanojsonc_parse_object(json, NULL, &person, callback);
    printf("Name: %s, Age: %d", person.name, person.age); // Name: John Doe, Age: 25

    free(person.name);
    return 0;
}
```

## Parsing JSON Array
```c
#include <nanojsonc/parser.h>

struct Hobby {
    char *name;
    struct Hobby *next;
};

static void callback(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    struct Hobby **hobbies = object;

    struct Hobby *hobby = malloc(sizeof(struct Hobby));
    hobby->name = strdup(value);
    hobby->next = NULL;

    struct Hobby **cursor;
    for (cursor = hobbies; *cursor; cursor = &(*cursor)->next);

    *cursor = hobby;
}

int main(void) {
    char *json = "[\"Reading\", \"Hiking\", \"Cooking\"]";

    struct Hobby *hobbies = NULL;
    nanojsonc_parse_array(json, "hobbies", &hobbies, callback);

    for (struct Hobby **cursor = &hobbies; *cursor; cursor = &(*cursor)->next)
        printf("%s ", (*cursor)->name); // Reading Hiking Cooking

    return 0;
}
```

## Installation: VCPKG

```commandline
vcpkg install nanojsonc
```
### CMakeLists.txt
```cmake
find_package(nanojsonc CONFIG REQUIRED)
target_link_libraries(main PRIVATE nanojsonc::nanojsonc)
```

Configure CMake Options: [vcpkg in CMake projects](https://learn.microsoft.com/en-us/vcpkg/users/buildsystems/cmake-integration)
```
-DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
```

If you receive malloc: nano zone, set environment variable `MallocNanoZone=0`

## Installation: Conan

### CMakeLists.txt

```cmake
# First, tell CMake to find the package.
# Conan will install the packages so that CMake can find it:

find_package(nanojsonc)

# Then, link your executable or library with the corresponding package targets:

target_link_libraries(your_exe_or_lib_name nanojsonc::nanojsonc)
```

## Installation: Direct

Override `find_package`
```cmake
set(as_subproject nanojsonc)

macro(find_package)
    if(NOT "${ARG0}" IN_LIST as_subproject)
        _find_package(${ARGV})
    else()
        add_subdirectory(${ARG0})
    endif()
endmacro()
```
[//]: # (gcc -g demo.c src/*.c -Iinclude/ -g -D NANOJSONC_KEY_SIZE=4)

## Examples

For a complete demo of how to use nanoJSONc, please refer to the following 

* [Examples](https://github.com/open-source-patterns/nanojsonc/tree/main/example)
* [API Docs](https://github.com/open-source-patterns/nanojsonc/blob/main/include/parser.h)

## Reference
* [Effective CMake](https://www.youtube.com/watch?v=rLopVhns4Zs)
* [It's Time To Do CMake Right](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right)

## Status

Production - [Version 1.2.0](https://github.com/open-source-patterns/nanojsonc/blob/master/VERSION)

## License

BSD 3-Clause License

* Copyright © 2023 [Saad Shams](https://www.linkedin.com/in/muizz/)
* Copyright © 2023 [Open Source Patterns Inc.](https://github.com/open-source-patterns)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
