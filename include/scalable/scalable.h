
#pragma once

#include<bits/stdc++.h>
#include<type_traits>
#include"_fun.h"
#include"_Span.h"

namespace scalable {
template<typename Q>
struct is_pair{};

template<typename Key, typename Val> 
struct is_pair<std::pair<Key, Val>> {
    typedef std::map<Key, Val> map_type;
    typedef std::unordered_map<Key, Val> unordered_map_type;
};
// *************************************************





// helper functions to avoid direct template specialization neccessary for classes.
template<typename T, template<typename, typename...> class V, typename... Args>
Span<T, V, Args...> span(V<T,Args...> v) {
	return Span<T,V,Args...>(v);
}
template<typename K, typename T, template<typename, typename, typename...> class V, typename... Args>
SpanMap<K, T, V, Args...> spanMap(const V<K,T,Args...> & v) {
	return SpanMap<K, T,V,Args...>(v);
}

template<typename T, template<typename, typename...> class V, typename... Args>
template<typename Q, typename MapType>
SpanMap<typename MapType::key_type, 
    typename MapType::mapped_type, 
    std::map,
    typename MapType::key_compare,
    typename MapType::allocator_type
    > 
Span<T,V,Args...>::toTreeMap() {
    MapType resultContainer;
    for(const auto & c: container) {
        resultContainer[c.first] = c.second;
    }
    return spanMap(resultContainer);
}


template<typename T, template<typename, typename...> class V, typename... Args>
template<typename Q, typename MapType>
SpanMap<typename MapType::key_type, 
    typename MapType::mapped_type, 
    std::unordered_map,
    typename MapType::hasher,
    typename MapType::key_equal,
    typename MapType::allocator_type
> 
Span<T,V,Args...>::toHashMap() {
    MapType resultContainer;
    for(const auto & c: container) {
        resultContainer[c.first] = c.second;
    }
    return spanMap(resultContainer);
}
} // namespace scalable
