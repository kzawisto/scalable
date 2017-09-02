#pragma once

namespace scalable {
template<typename Q>
struct is_pair{};

template<typename Key, typename Val> 
struct is_pair<std::pair<Key, Val>> {
    typedef std::map<Key, Val> map_type;
    typedef std::unordered_map<Key, Val> unordered_map_type;
};

} //namespace scalable
