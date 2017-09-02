#pragma once
#include<vector>
#include<set>
#include<map>
#include<unordered_map>
#include<list>
#include<scalable/_is_pair.h>
namespace scalable {
template<typename K, typename T, template<typename,typename, typename...> class V, typename... Args>
class CollectMap ;

// Wrapper for container holding an array of values
// such as vector, set etc.
template<typename T, template<typename, typename...> class V, typename... Args>
class Collect {
public:
	V<T,Args...> container;
    typedef std::vector<T> VecT;
    typedef std::set<T> TreeSetT;
    typedef std::list<T> ListT;
    typedef std::unordered_set<T> HashSetT;
	Collect(){

	}
	Collect(const V<T,Args...> & vec) : container(vec.begin(), vec.end()) {

	}
	V<T,Args...> get() {
		return container;
	}
    template<typename Q>
	Collect<Q,V,Args...> map ( std::function<Q(T)> f) {
		Collect<Q,V,Args...> ret;
		for(const auto & el: container) {
			ret.container.insert(ret.container.end(), f(el));
		}
		return ret;
	}
	template<typename W>
	Collect<typename W::value_type,V,Args...> flatMap ( std::function<W(T)> f) {
		Collect<typename W::value_type,V,Args...> ret;
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

    template<typename Q=T, typename MapType= typename is_pair<Q>::map_type>
    CollectMap<typename MapType::key_type, 
        typename MapType::mapped_type, 
        std::map,
        typename MapType::key_compare,
        typename MapType::allocator_type
    > toTreeMap();

    template<typename Q=T, typename MapType= typename is_pair<Q>::unordered_map_type>
    CollectMap<typename MapType::key_type, 
        typename MapType::mapped_type, 
        std::unordered_map,
        typename MapType::hasher,
        typename MapType::key_equal,
        typename MapType::allocator_type
    > toHashMap();


	Collect<T,V,Args...> filter ( std::function<bool(T)> f) {
		Collect<T,V,Args...> ret;
		for(const auto & el: container) {
			if(f(el))
				ret.container.insert(ret.container.end(), el);
		}
		return ret;
	}
};

template<typename T, template<typename, typename...> class V, typename... Args>
Collect<T, V, Args...> collect(V<T,Args...> v) {
	return Collect<T,V,Args...>(v);
}

} //namespace scalable
