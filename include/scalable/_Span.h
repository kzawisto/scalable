#pragma once
#include<vector>
#include<set>
#include<map>
#include<unordered_map>
#include<list>
namespace scalable {
template<typename K, typename T, template<typename,typename, typename...> class V, typename... Args>
class SpanMap ;

// Wrapper for container holding an array of values
// such as vector, set etc.
template<typename T, template<typename, typename...> class V, typename... Args>
class Span {
public:
	V<T,Args...> container;
    typedef std::vector<T> VecT;
    typedef std::set<T> TreeSetT;
    typedef std::list<T> ListT;
    typedef std::unordered_set<T> HashSetT;
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

    template<typename Q=T, typename MapType= typename is_pair<Q>::map_type>
    SpanMap<typename MapType::key_type, 
        typename MapType::mapped_type, 
        std::map,
        typename MapType::key_compare,
        typename MapType::allocator_type
    > toTreeMap();

    template<typename Q=T, typename MapType= typename is_pair<Q>::unordered_map_type>
    SpanMap<typename MapType::key_type, 
        typename MapType::mapped_type, 
        std::unordered_map,
        typename MapType::hasher,
        typename MapType::key_equal,
        typename MapType::allocator_type
    > toHashMap();


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

    typedef std::vector<std::pair<K,T>> PairVec;
	typedef Span<typename PairVec::value_type, std::vector, typename PairVec::allocator_type> PairVecSpan;
    PairVecSpan toVec (){
        PairVecSpan ret;
        for(const auto & c : _map) 
        {
			ret.container.push_back(c);
		}
		return ret;
	}

    typedef std::list<std::pair<K, T>> PairList;
	Span<typename PairList::value_type, std::list, typename PairList::allocator_type>
        toList (){
        Span<typename PairList::value_type, std::list, typename PairList::allocator_type> ret;
        for(const auto & c : _map) 
        {
			ret.container.push_back(c);
		}
		return ret;
	}
    
    typedef std::set<std::pair<K,T>> PairSet;
	typedef Span<typename PairSet::value_type, 
        std::set, 
        typename PairSet::key_compare,
        typename PairSet::allocator_type> pair_span_set;
    pair_span_set st1;
    
    pair_span_set toTreeSet (){
        pair_span_set ret;
        for(const auto & c : _map) 
        {
			ret.container.insert(c);
		}
		return ret;
	}
};


} //namespace scalable
