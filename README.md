# Scalable

## Sugar for C++ STL containers inspired by scala.


### About

Scalable is a C++ library that wraps STL with immutable interface modeled after Scala lanugage. Works for dynamically as well as statically (functor/lambdas) called functionoids, which gives it efficiency on par with  handrolled control flow.
See the following example and unit tests for reference. Code is available on [github](https://github.com/kzawisto/scalable) and shared under MIT License.


You may want to take a look at my another take at the topic [glues](https://github.com/kzawisto/glues), a C++ library generating
lazy evaluation procedures at compile time for maximum performance, as well as [mc](htts://github.com/kzawisto/mc):
a Python library with similiar functionality.
```c++
collect(std::set<int>{1,2,3,4,5})
					.map([](int i ) {
                                return i * i; 
                    })
                    .filter([](int j) {
                                return j < 18;
                    })
                    .flatMap([](int i) {
                                return std::set<int>{ i, i + 1, i + 2};
                    })
                    .groupBy([](int j) {
                                return j % 2 == 0;
                    })
                    .map([](bool v, std::vector<int> l) {
                                    return collect(l)
                                        .map([](int i) {
                                                return std::to_string(i);
                                        })
                                        .reduce(fun([] (std::string lhs, std::string rhs){
                                                return lhs + "_" + rhs;
                                                    }));
                                })
                    .get()
                    .at(true);
```


### Installation

C++11 or newer required, tested with g++-5.2.

Header only library, include scalable/scalable.h. See unit tests in test.cxx for reference. Build tests with make, googletest is needed for that.

