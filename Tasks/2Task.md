# Programming Language


## The C++ Programming Language

[C++](http://www.cplusplus.com/) is a popular, general-purpose programming language.
It is often considered an intermediate-level language because it admits both high-level and low-level paradigms.
C++ adds object-oriented programming features to C.
It supports classes and their main attributes: abstraction, encapsulation, inheritance, and polymorphism.
Technically, C++ incorporates the C standard library, with slight modifications.
Pertinent documentation about this language can be found on the [C++ resources network](http://www.cplusplus.com/).

Many compilers are available for C++, including Visual Studio (Microsoft) and GCC (GNU Project).
The C/C++ syntax forms the basis for software development on several microcontroller platforms.
Texas A&amp;M University provides access to several books on C++ programming.
For example, The C++ programming language is accessible online through campus.

http://proquest.safaribooksonline.com/9780133522884

To build and debug a C++ projects, a toolchain is required.
Each platform that runs a C/C++ Development Tools (CDT) requires a unique installation process.

### Action Items

* __Download and Install__: A C++ Toolchain.
  1. __Windows Option 1__:
  Visual Studio, which is available for free to ECE students through Microsoft [DreamSpark](https://www.dreamspark.com/Institution/Access.aspx).
  2. __Windows Option 2__:
  [MinGW](http://sourceforge.net/projects/mingw/files/) using the `mingw-get-inst` package and default directory.
* __MacOS X__:
Apple GNU toolchain included in the [Xcode IDE](http://developer.apple.com).
* __GNU/Linux__:
Most GNU/Linux distributions provide the [GNU toolchain](https://gcc.gnu.org/).
* __Create, Build, and Run__:
HelloWorld.


## CMake

[CMake](http://www.cmake.org/) is a cross-platform, open-source build system.
It is a family of tools designed to build, test and package software.
CMake is used to control the software compilation process using simple platform and compiler independent configuration files.
CMake generates native makefiles and workspaces that can be used in various compiler environments, and supports cross-platform development.

### Action Items

* __Read__: The [CMake tutorial](http://cmake.org/cmake/help/cmake_tutorial.html).
* __Peruse__: The Introduction to [CMake Course](http://cmake.org/cmake/resources/webinars.html).
* __Download and Install__: [CMake](http://cmake.org/cmake/resources/software.html).
* __Create, Build, and Run__: HelloWorld using CMake.

### Structure

Version control systems form a great paradigm to facilitate collaborative projects.
Still, one of the many difficulties encountered in large projects is the fact that most programmers have a strong preference for a programming style.
Following a few elementary guidelines can greatly simplify team work.
A short list of generally applicable rules can be found below.

* __Main__: Only create one `main()` class to avoid confusing automated builders.
This will prevent the `duplicate symbol` error that may otherwise occur at `link` time.
* __Structure__: Declare your classes in `header` files and define your classes in `source` files in a structured manner.
* __Hierarchy__: For simple projects, place all your files in a same directory, e.g., `src`.
* __Documentation__: Annotate your code appropriately, and remember that comments should be decipherable by someone else than the original programmer.
* __Libraries__: Only include cross-platform libraries for greater compatibility, e.g., `Boost`.

