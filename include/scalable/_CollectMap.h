#pragma once
#include<scalable/_Collect.h>
namespace scalable {

template<typename K, typename T, template<typename,typename, typename...> class V, typename... Args>
class CollectMap {

public:
	V<K,T,Args...> _map;
	CollectMap(const V<K,T,Args...> & _m ) : _map(_m.begin(), _m.end() ){

	}
	CollectMap() {

	}
	const V<K,T,Args...> & get() {
		return _map;
	}
	template<typename Q>
	CollectMap<K,Q,V,Args...> map ( std::function<Q(T)> f) {
		CollectMap<K,Q,V,Args...> ret;
		for(const auto & el: _map) {
			ret[el.first] = f(el.second);
		}
		return ret;
	}
	template<typename Q>
	CollectMap<K,Q,V,Args...> map ( std::function<Q(K,T)> f) {
		CollectMap<K,Q,V,Args...> ret;
		for(const auto & el: _map) {
			ret._map[el.first] = f(el.first, el.second);
		}
		return ret;
	}

	CollectMap<K,T,V,Args...> filter ( std::function<bool(T)> f) {
		CollectMap<K, T,V,Args...> ret;
		for(const auto & el: _map) {
			if(f(el.second)) {
				ret._map[el.first] = el.second;
			}
		}
		return ret;
	}

    typedef std::vector<std::pair<K,T>> PairVec;
	typedef Collect<typename PairVec::value_type, std::vector, typename PairVec::allocator_type> PairVecCollect;
    PairVecCollect toVec (){
        PairVecCollect ret;
        for(const auto & c : _map) 
        {
			ret.container.push_back(c);
		}
		return ret;
	}

    typedef std::list<std::pair<K, T>> PairList;
	Collect<typename PairList::value_type, std::list, typename PairList::allocator_type>
        toList (){
        Collect<typename PairList::value_type, std::list, typename PairList::allocator_type> ret;
        for(const auto & c : _map) 
        {
			ret.container.push_back(c);
		}
		return ret;
	}
    
    typedef std::set<std::pair<K,T>> PairSet;
	typedef Collect<typename PairSet::value_type, 
        std::set, 
        typename PairSet::key_compare,
        typename PairSet::allocator_type> pair_collect_set;
    pair_collect_set st1;
    
    pair_collect_set toTreeSet (){
        pair_collect_set ret;
        for(const auto & c : _map) 
        {
			ret.container.insert(c);
		}
		return ret;
	}
};

template<typename K, typename T, template<typename, typename, typename...> class V, typename... Args>
CollectMap<K, T, V, Args...> collectMap(const V<K,T,Args...> & v) {
	return CollectMap<K, T,V,Args...>(v);
}
}
