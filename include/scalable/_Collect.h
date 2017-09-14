#pragma once
#include<vector>
#include<set>
#include<map>
#include<unordered_map>
#include<list>
#include<scalable/_is_pair.h>
namespace scalable
{
template<typename Key, typename Value, template<typename,typename, typename...> class Container, typename... Args>
class CollectMap ;

// Wrapper for container holding an array of values
// such as vector, set etc.
template<typename Value, template<typename, typename...> class Container, typename... Args>
class Collect
{
public:
    Container<Value,Args...> container;
    typedef std::vector<Value> VecT;
    typedef std::set<Value> TreeSetT;
    typedef std::list<Value> ListT;
    typedef std::unordered_set<Value> HashSetT;
    Collect() {

    }
    Collect ( const Container<Value,Args...> & vec ) : container ( vec.begin(), vec.end() ) {

    }
    Container<Value,Args...> get() {
        return container;
    }
    template<typename Q>
    Collect<Q,Container,Args...> map ( std::function<Q (Value) > f ) {
        Collect<Q,Container,Args...> ret;
        for ( const auto & el: container ) {
            ret.container.insert ( ret.container.end(), f ( el ) );
        }
        return ret;
    }
    template<typename ReturnType>
    Collect<typename ReturnType::value_type,Container,Args...> flatMap (
        std::function<ReturnType (Value) > f
    ) {
        Collect<typename ReturnType::value_type,Container,Args...> ret;
        for ( const auto & el: container ) {
            ReturnType tmp = f ( el );
            for ( const auto & el2 : tmp ) {
                ret.container.insert ( ret.container.end(), el2 );
            }
        }
        return ret;
    }
    template<typename ReturnType>
    Collect<typename ReturnType::value_type,Container,Args...> flatMap (
        std::function<ReturnType (const Value &) > f
    ) {
        std::function<ReturnType(Value)> newF = f;
        return flatMap(newF);
    }
    template<typename Q>
    Collect<Q,Container,Args...> scanLeft ( Q initial, std::function<Q ( Q, Value ) > f ) {
        Collect<Q,Container,Args...> ret;
        for ( const auto & el: container ) {
            initial = f ( initial, el );
            ret.container.insert ( ret.container.end(), initial );

        }
        return ret;
    }
    Value reduce ( std::function<Value ( Value, Value ) > f ) {
        auto it = container.begin();
        Value result = *it;
        it++;
        for ( ; it != container.end(); it++ ) {
            result = f ( result, *it );
        }
        return result;
    }

    template<typename Q=Value, typename MapType= typename is_pair<Q>::map_type>
    CollectMap<typename MapType::key_type,
               typename MapType::mapped_type,
               std::map,
               typename MapType::key_compare,
               typename MapType::allocator_type
               > toTreeMap();

    template<typename Q=Value, typename MapType= typename is_pair<Q>::unordered_map_type>
    CollectMap<typename MapType::key_type,
               typename MapType::mapped_type,
               std::unordered_map,
               typename MapType::hasher,
               typename MapType::key_equal,
               typename MapType::allocator_type
               > toHashMap();


    Collect<Value,Container,Args...> filter ( std::function<bool ( Value ) > f ) {
        Collect<Value,Container,Args...> ret;
        for ( const auto & el: container ) {
            if ( f ( el ) ) {
                ret.container.insert ( ret.container.end(), el );
            }
        }
        return ret;
    }


    template<typename Q=Value, typename V, typename MapType= std::map<V,std::list<Q> >>
    CollectMap<typename MapType::key_type,
               typename MapType::mapped_type,
               std::map,
               typename MapType::key_compare,
               typename MapType::allocator_type
               > groupBy ( std::function<V ( Q ) > f );
};

template<typename Value, template<typename, typename...> class Container, typename... Args>
Collect<Value, Container, Args...> collect ( Container<Value,Args...> v )
{
    return Collect<Value,Container,Args...> ( v );
}

} //namespace scalable
