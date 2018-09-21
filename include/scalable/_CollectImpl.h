#pragma once

// impl header, should not be included on its own
#include<scalable/_Collect.h>
#include<scalable/_CollectMap.h>
#include<scalable/_traits.h>
namespace scalable
{


template<typename Value, template<typename, typename...> class Container, typename... Args>
template<typename Q, typename MapType>
CollectMap<typename MapType::key_type,
           typename MapType::mapped_type,
           std::map,
           typename MapType::key_compare,
           typename MapType::allocator_type
           >
           Collect<Value, Container ,Args...>::toTreeMap()
{
    MapType resultContainer;
    for ( const auto & c: container ) {
        resultContainer[c.first] = c.second;
    }
    return collectMap ( resultContainer );
}


template<typename Value, template<typename, typename...> class Container, typename... Args>
template<typename Q, typename MapType>
CollectMap<typename MapType::key_type,
           typename MapType::mapped_type,
           std::unordered_map,
           typename MapType::hasher,
           typename MapType::key_equal,
           typename MapType::allocator_type
           >
           Collect<Value, Container, Args...>::toHashMap()
{
    MapType resultContainer;
    for ( const auto & c: container ) {
        resultContainer[c.first] = c.second;
    }
    return collectMap ( resultContainer );
}

template<typename Value, template<typename, typename...> class Container, typename... Args>
template<typename Op, typename MapType>
CollectMap<typename MapType::key_type,
           typename MapType::mapped_type,
           std::map,
           typename MapType::key_compare,
           typename MapType::allocator_type
           > Collect<Value, Container, Args...>::groupBy ( Op f )
{
    MapType resultContainer;
    for(const auto & elem: container) {
        resultContainer[f(elem)].push_back(elem);
    }
    return collectMap(resultContainer);
    
}

} // namespace scalable
