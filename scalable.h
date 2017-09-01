
#pragma once

#include<bits/stdc++.h>
#include<type_traits>



// Lambdas are sugar for functor class in c++ 11. To use them efficiently
// we need to pass them to functions somehow, and using auto makes type deduction impossible.
// We cast it to std::function with fun() function instead.

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
// *************************************************

// This is to concatenate functions easily for more memory efficiency
template<typename T1, typename T2, typename T3>
std::function<T1(T3)> operator&( const std::function<T1(T2)> & f1, const std::function<T2(T3)> & f2) {
	return fun([f1,f2]( T1 arg) -> T3 {
		return f1(f2(arg));
	});
}

// Wrapper for container holding an array of values (plus extra structure)
// such as vector, set or in future deque, list, undordered_set.
// This is a small PoC, only vector and set are supported, and I will replace this soon.

template<typename K, typename T, template<typename,typename, typename...> class V, typename... Args>
class SpanMap ;

template<typename T, template<typename, typename...> class V, typename... Args>
class Span {
public:
	V<T,Args...> container;
	Span(){

	}
	Span(const V<T,Args...> & vec) : container(vec.begin(), vec.end()) {

	}
	V<T,Args...> get() {
		return container;
	}
    template<typename Q>
	Span<Q,V,Args...> map ( std::function<Q(T)> f) {
		Span<Q,V,Args...> ret;
		for(const auto & el: container) {
			ret.container.insert(ret.container.end(), f(el));
		}
		return ret;
	}
	template<typename W>
	Span<typename W::value_type,V,Args...> flatMap ( std::function<W(T)> f) {
		Span<typename W::value_type,V,Args...> ret;
		for(const auto & el: container) {
			W tmp = f(el);
			for(const auto & el2 : tmp)
			ret.container.insert(ret.container.end(), el2);
		}
		return ret;
	}


	T reduce(std::function<T(T,T)> f) {
		auto it = container.begin();
		T result = *it;
		it++;
		for(;it != container.end();it++) {
			result = f(result, *it);
		}
		return result;
	}

    template<typename Q>
    struct is_pair{};
    
    template<typename Key, typename Val> 
    struct is_pair<std::pair<Key, Val>> {
        typedef std::map<Key, Val> map_type;
        typedef std::unordered_map<Key, Val> unordered_map_type;
    };
    template<typename Q=T>
    struct is_pair<Q>::map_type toTreeMap();


	Span<T,V,Args...> filter ( std::function<bool(T)> f) {
		Span<T,V,Args...> ret;
		for(const auto & el: container) {
			if(f(el))
				ret.container.insert(ret.container.end(), el);
		}
		return ret;
	}
    
    template<typename Q=T, typename isPair = is_pair<T>>
    void doSth() {

    }
    

};

// As above, but for map and unordered map this time.
template<typename K, typename T, template<typename,typename, typename...> class V, typename... Args>
class SpanMap {

public:
	V<K,T,Args...> _map;
	SpanMap(const V<K,T,Args...> & _m ) : _map(_m.begin(), _m.end() ){

	}
	SpanMap() {

	}
	const V<K,T,Args...> & get() {
		return _map;
	}
	template<typename Q>
	SpanMap<K,Q,V,Args...> map ( std::function<Q(T)> f) {
		SpanMap<K,Q,V,Args...> ret;
		for(const auto & el: _map) {
			ret[el.first] = f(el.second);
		}
		return ret;
	}
	template<typename Q>
	SpanMap<K,Q,V,Args...> map ( std::function<Q(K,T)> f) {
		SpanMap<K,Q,V,Args...> ret;
		for(const auto & el: _map) {
			ret._map[el.first] = f(el.first, el.second);
		}
		return ret;
	}

	SpanMap<K,T,V,Args...> filter ( std::function<bool(T)> f) {
		SpanMap<K, T,V,Args...> ret;
		for(const auto & el: _map) {
			if(f(el.second)) {
				ret._map[el.first] = el.second;
			}
		}
		return ret;
	}
    typedef std::vector<std::pair<K,T>> pairVec;
	Span<typename pairVec::value_type, std::vector, typename pairVec::allocator_type>
        toVec (){
        Span<typename pairVec::value_type, std::vector, typename pairVec::allocator_type> ret;
        for(const auto & c : _map) 
        {
			ret.container.push_back(c);
		}
		return ret;
	}
    typedef std::set<std::pair<K,T>> pairSet;
	typedef Span<typename pairSet::value_type, 
        std::set, 
        typename pairSet::key_compare,
        typename pairSet::allocator_type> pair_span_set;
    pair_span_set st1;
    
    pair_span_set toSet (){
        pair_span_set ret;
        for(const auto & c : _map) 
        {
			ret.container.insert(c);
		}
		return ret;
	}
};

template<typename T, template<typename, typename...> class V, typename... Args>
template<typename Q=T>
struct is_pair<Q>::map_type Span<T,V,Args...>::toTreeMap() {
    typename is_pair<Q>::map_type resultContainer;
    for(const auto & c: container) {
        resultContainer[c.first] = c.second;
    }
    return spanMap(resultContainer);
}

// helper functions to avoid direct template specialization neccessary for classes.
template<typename T, template<typename, typename...> class V, typename... Args>
Span<T, V, Args...> span(V<T,Args...> v) {
	return Span<T,V,Args...>(v);
}
template<typename K, typename T, template<typename, typename, typename...> class V, typename... Args>
SpanMap<K, T, V, Args...> spanMap(const V<K,T,Args...> & v) {
	return SpanMap<K, T,V,Args...>(v);
}

