
#pragma once
#include<type_traits>

template<typename Op, typename Val>
using func_ret = decltype(std::declval<Op>()(std::declval<Val>()));

template<typename Op, typename Val1, typename Val2>
using func_ret_binary = decltype(std::declval<Op>()(std::declval<Val1>(), std::declval<Val2>()));

