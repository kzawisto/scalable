#pragma once

// impl header, should not be included on its own

namespace scalable {


template<typename T, template<typename, typename...> class V, typename... Args>
template<typename Q, typename MapType>
CollectMap<typename MapType::key_type, 
    typename MapType::mapped_type, 
    std::map,
    typename MapType::key_compare,
    typename MapType::allocator_type
    > 
Collect<T,V,Args...>::toTreeMap() {
    MapType resultContainer;
    for(const auto & c: container) {
        resultContainer[c.first] = c.second;
    }
    return collectMap(resultContainer);
}


template<typename T, template<typename, typename...> class V, typename... Args>
template<typename Q, typename MapType>
CollectMap<typename MapType::key_type, 
    typename MapType::mapped_type, 
    std::unordered_map,
    typename MapType::hasher,
    typename MapType::key_equal,
    typename MapType::allocator_type
> 
Collect<T,V,Args...>::toHashMap() {
    MapType resultContainer;
    for(const auto & c: container) {
        resultContainer[c.first] = c.second;
    }
    return collectMap(resultContainer);
}
} // namespace scalable
