# Scalable

## Sugar for C++ STL containers inspired by scala.
### Important:

If you plan to use this in any way, then here you can find technically superior alternative to this little library, it is more elegant, less verbose and highly efficient as it avoids pointer calls and excessive memory use. 

https://github.com/kzawisto/glues


### TL DR

Scalable is a C++ library that allows to write complex container operations in simplier and more readable way. It is inspired by Scala language. It provides higher level operations on containers and operates on immutable objects, making coding easier and doing mistakes harder. See the following example and unit tests for reference. Code is available on [github](https://github.com/kzawisto/scalable) and shared under MIT License.
```c++
collect(std::set<int>{1,2,3,4,5})
    .map(fun([](int i ) {
                return i * i;
    }))
    .filter(fun([](int j) {
                return j < 18;
    }))
    .flatMap(fun([](int i) {
                return std::set<int>{ i, i + 1, i + 2};
    }))
    .groupBy(fun([](int j) {
                return j % 2 == 0;
    })) 
    .map(fun([](bool v, std::list<int> l) {
                    return collect(l)
                        .map(fun([](int i) {
                                std::stringstream ss; 
                                ss<< i;
                                return ss.str();
                        }))
                        .reduce(fun([] (std::string lhs, std::string rhs){
                                return lhs + "_" + rhs;
                        }));
                }))
    .get()
    .at(true);

```


### Installation

C++11 or newer required, tested with g++-5.2 and clang 3.8.

Header only library, include scalable/scalable.h. See unit tests in test.cxx for reference. You could build them executing make in project dir,googletest is needed for that.

