

#pragma once
#include<functional>
#include<type_traits>




// Lambdas are sugar for functor class in c++ 11.
// Here cast it to std::function with fun() function instead.

namespace scalable {
template <typename F>
struct LambdaToFunction
: public LambdaToFunction<decltype(&F::operator())>
{

};

template <typename Cls, typename Ret, typename... Args>
struct LambdaToFunction<Ret(Cls::*)(Args...) const>
{
    typedef std::function<Ret(Args...)> f;
};


template <typename F>
typename LambdaToFunction<F>::f
fun (const F& lambda)
{
    return typename LambdaToFunction<F>::f(lambda);
}

// Function concatenation
template<typename T1, typename T2, typename T3>
std::function<T1(T3)> operator&( const std::function<T1(T2)> & f1, const std::function<T2(T3)> & f2) {
	return fun([f1,f2]( T1 arg) -> T3 {
		return f1(f2(arg));
	});
}

} // namespace scalable
