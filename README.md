Danet (Distributed Asynchronous Network)
========================================

Summary
-------

Danet library is a C++ library that makes easier to write network modules in your programs.

Building library
----------------

### Requirements

You have to have installed these programs:

* cmake
* g++ (>= 4.7)
* doxygen

And these libraries (with development packages):

* libboost-system

### Compiling

Firstly you have to configure the library.

```bash
cd build_directory
cmake source_directory
```

`build_directory` is a directory where all build files will be put.
`source_directory` is a directory which contain source files.

You can add option `-DMAKE_DOC` to specify if the documentation should be made.
Eg. `cmake source_directory -DMAKE_DOC=on` tells that the documentation should be made.
The opposite value is `off`.

At the end you have to compile the library.
In the build directory execute:

```bash
make
```

### Installing

To install the library, execute in build directory:

```bash
make install
```

#### For Linux users:

You can also check if the library is in your Linux repository.

Usage
-----

To use the library in your CMake project simply add:

```cmake
find_package(Danet)
```

There are available variebles now: `${DANET_INCLUDE_DIRS}` and `${DANET_LIBRARIES}`.