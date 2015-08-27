# Application Graders

This C++ application can be employed to assign graders during peer grading activities.
It essentially creates a derangement of the names using the rejection method.

## CMake Command Line Build

```
cd ~/GitCourseReps/ECEN489-Fall2015/Miscellaneous/Graders
mkdir build
cd build 
cmake ../
make
```

## In-Place clang++ Build

```
clang++ -std=c++11 graders.cpp -o graders
```


## In-Place g++ Build

```
g++ -std=c++11 graders.cpp -o graders
```
