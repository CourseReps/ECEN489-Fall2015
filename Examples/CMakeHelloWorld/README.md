# CMake on the Command Line

For small projects, it is possible to employ CMake to build applications in a non-interactive manner.
It may be a good idea to use separate build directories.

```
mkdir build
cd build 
cmake ../
make
```

In any case, do not commit your build artifacts to the _git_ repository.
