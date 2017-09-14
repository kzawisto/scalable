# Scalable

## Sugar for C++ STL containers inspired by scala.

### TL DR

Scalable is a C++ library that allows to write complex container operations in simplier and more readable way. It is inspired by Scala language. It provides functional implementations of higher level operations on containers and operates on immutable object, thus limiting space for errors and making coding easier and faster. See the following example and unit tests for reference. Code is available on [github](https://github.com/kzawisto/scalable) and shared under MIT License.
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

### Motivation

This is about operations on collections that involve some iteration and return an another collection or a single value. It was noticed some time ago that usually such operations can be decomposed into chains of some more general abstractions of control flow that apply a function to the values in some specified manner. The most common are map (e.g. for set of x produce set of x doubled), reduce (e.g. produce sum of elements)  and filter (e.g. find numbers in a list that are odd). If you are not familiar with them you could read some details [here on MIT website](http://web.mit.edu/6.005/www/fa15/classes/25-map-filter-reduce/).


I will give a simple example of such operation in Scala: let us, for each number from 0 to 10 calculate the square, and sum squares that are less than 50:
```scala
val result = (0 to 10)
    .map(x => x * x)
    .filter(_ < 50)
    .reduce(_ + _)
println(result)
```
(For those not familiar with Scala, x => x * x or _ + _ are just lambda functions, the former computes square and the latter computes the sum). One can say that this type of of code is very nice to read and understand compared to cryptic chunk of ifs and fors. We could see immediately the high level control flow, we start with squaring and end with computing the sum, no going backwards, no jumping. 
Each line tells us immediately what it does and each line does one simple thing. Under the hood some iterations happen and some collections are created and destroyed but we don't care at this point, all we need is the one at the end and the other at the beginning (performance "suffers" ofc but we will come to that later).

C++ Standard Template Library has it's map and reduces too but there are major differences. They are called transform, accumulate and copy_if. The following code shows our example written with use of them.
```c++
std::vector<int> vec(11);
std::iota(vec.begin(), vec.end(), 0); 
std::vector<int> result(11);
std::transform(vec.begin(), vec.end(), result.begin(), [] (int x) {
            return x * x;
        }); 
std::vector<int> result2;
std::copy_if(result.begin(), result.end(), std::back_inserter(result2), [] (int x) {
            return x < 50; 
        }); 
std::cout<< std::accumulate(result2.begin(), result2.end(), 0, [] (int a, int b) {
            return a + b;
        }); 
```
As you can imagine it took me more time to write this in C++ than in Scala, though I have significantly more experience with the former than the latter. The good thing is that we still can deduce what the programm does just by skimming, but at a glance there is more details to think about and more space for mistakes. This time we operate on pairs of iterators, i.e. single object holding values was replaced by two "things" that have values in between. Vectors for results are declared explicitely (and made to exist till the end of the code block). Transform operation overrides the content of existing vector while copy_if appends at the end of the one (std::back_inserter), and this difference is definitely not explicit. Reading is still more pleasant than writing, as signatures of three or four argument functions are by no means obvious, and you could easily make mistake by just switching the order. Considering that input is read from one pair of given iterators to another, this does not necessarily needs to get caught at compile-time, potentially producing a time consuming bug. One of leading authorities on Agile software engineering, R.C. Martin stresses out frequently that the function should be explicit in what they do. should have small number of arguments, and should return their result in return value, not output argument.  

Obviously there are deep reasons why C++ is built upon completely different principles. Control over allocating memory can be needed, for instance we could do many transforms in place to save resources. Iterators can be just anything with incrementation, comparison and dereference. Especially they can be ordinary pointers, so we could use STL algorithms on raw memory. 

In my humble opinion most of the time for each small chunk of as-fast-as-possible code you would have a large pile of complex code than can be slow, where few miliseconds for one less thing to worry about would be a huge bargain. One could for instance think of replacing interface STL provides , sweeping all the gory details under the rug.

### Installation

C++11 or newer required, tested with g++-5.2 and clang 3.8.

Header only library, include scalable/scalable.h. See unit tests in test.cxx for reference. You could build them executing make in project dir,googletest is needed for that.

