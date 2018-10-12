#pragma once
#include<scalable/_Collect.h>
#include<scalable/_traits.h>
namespace scalable
{

template<typename K, typename T, template<typename,typename, typename...> class V, typename... Args>
class CollectMap
{

public:
    V<K,T,Args...> _map;
    CollectMap ( const V<K,T,Args...> & _m ) : _map ( _m.begin(), _m.end() ) {

    }
    CollectMap() {

    }
    const V<K,T,Args...> & get() {
        return _map;
    }

    template<typename Op>
    using ret_type = func_ret_binary<Op,K,T>;


    template<typename Op>
    using RetMap = CollectMap<K,ret_type<Op>,V>; //V<K, ret_type<Op>>;//decltype(get_ret_type(std::declval<Op>()));
    using ThisMap = CollectMap<K, T, V, Args...>;

    template<typename Op>
    RetMap<Op> map ( Op f ) {
        RetMap<Op> ret;
        for ( const auto & el: _map ) {
            ret._map[el.first] = f ( el.first, el.second );

        }
        return ret;
    }


    template<typename Op>
    CollectMap<K,T,V,Args...> filter ( Op f ) {
        CollectMap<K, T,V,Args...> ret;
        for ( const auto & el: _map ) {
            if ( f ( el.first, el.second ) ) {
                ret._map[el.first] = el.second;
            }
        }
        return ret;
    }

    typedef std::vector<std::pair<K,T>> PairVec;
    typedef Collect<typename PairVec::value_type, std::vector, typename PairVec::allocator_type> PairVecCollect;

    PairVecCollect toVec () {
        PairVecCollect ret;
        for ( const auto & c : _map ) {
            ret.container.push_back ( c );
        }
        return ret;
    }

    typedef std::list<std::pair<K, T>> PairList;
    Collect<typename PairList::value_type, std::list, typename PairList::allocator_type>
    toList () {
        Collect<typename PairList::value_type, std::list, typename PairList::allocator_type> ret;
        for ( const auto & c : _map ) {
            ret.container.push_back ( c );
        }
        return ret;
    }

    typedef std::set<std::pair<K,T>> PairSet;
    typedef std::set<K> KeySet;
    typedef std::vector<T> ValuesVector;
    typedef Collect<typename PairSet::value_type,
            std::set,
            typename PairSet::key_compare,
            typename PairSet::allocator_type> PairCollectSet;

    PairCollectSet toTreeSet () {
        PairCollectSet ret;
        for ( const auto & c : _map ) {
            ret.container.insert ( c );
        }
        return ret;
    }

    typedef Collect<typename KeySet::value_type,
            std::set,
            typename KeySet::key_compare,
            typename KeySet::allocator_type> key_collect_set;

    key_collect_set getKeys() {
        key_collect_set ret;
        for ( const auto & c : _map ) {
            ret.container.insert ( c.first );
        }
        return ret;
    }


    typedef Collect<typename ValuesVector::value_type,
            std::vector,
            typename ValuesVector::allocator_type> ValuesCollectVector;


    ValuesCollectVector getValues() {
        ValuesCollectVector ret;
        for ( const auto & c : _map ) {
            ret.container.insert ( ret.container.end(), c.second );
        }
        return ret;
    }

};

template<typename K, typename T, template<typename, typename, typename...> class V, typename... Args>
CollectMap<K, T, V, Args...> collectMap ( const V<K,T,Args...> & v )
{
    return CollectMap<K, T,V,Args...> ( v );
}
}
