#pragma once
#include<vector>
#include<set>
#include<map>
#include<unordered_map>
#include<list>
#include<unordered_set>
#include<scalable/_is_pair.h>
#include<scalable/_traits.h>
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
    template<typename Op>
    Collect<func_ret<Op,Value>,Container,Args...> map ( Op f ) {
        Collect<func_ret<Op,Value>,Container,Args...> ret;
        for ( const auto & el: container ) {
            ret.container.insert ( ret.container.end(), f ( el ) );
        }
        return ret;
    }
    template<typename Op>
    Collect<typename func_ret<Op, Value>::value_type,Container,Args...> flatMap (
        Op f
    ) {
        Collect<typename func_ret<Op, Value>::value_type,Container,Args...> ret;
        for ( const auto & el: container ) {
            auto tmp = f ( el );
            for ( const auto & el2 : tmp ) {
                ret.container.insert ( ret.container.end(), el2 );
            }
        }
        return ret;
    }
    template<typename Q, typename Op>
    Collect<Q,Container,Args...> scanLeft ( Q initial, Op f ) {
        Collect<Q,Container,Args...> ret;
        for ( const auto & el: container ) {
            initial = f ( initial, el );
            ret.container.insert ( ret.container.end(), initial );

        }
        return ret;
    }
    template<typename Op>
    Value reduce ( Op f ) {
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


    template<typename Op>
    Collect<Value,Container,Args...> filter ( Op f ) {
        Collect<Value,Container,Args...> ret;
        for ( const auto & el: container ) {
            if ( f ( el ) ) {
                ret.container.insert ( ret.container.end(), el );
            }
        }
        return ret;
    }


    template<typename Op, typename MapType= std::map<func_ret<Op, Value>,std::vector<Value> >>
    CollectMap<typename MapType::key_type,
               typename MapType::mapped_type,
               std::map,
               typename MapType::key_compare,
               typename MapType::allocator_type
               > groupBy ( Op f );
};

template<typename Value, template<typename, typename...> class Container, typename... Args>
Collect<Value, Container, Args...> collect ( Container<Value,Args...> v )
{
    return Collect<Value,Container,Args...> ( v );
}


template<typename T>
using deref_iter = typename std::iterator_traits<T>::value_type;

template<typename T>
Collect<deref_iter<T>, std::vector, std::allocator<deref_iter<T>>> vectorize(T begin, T end) {
	return collect(
			std::vector<deref_iter<T>>(begin, end));
}

} //namespace scalable
