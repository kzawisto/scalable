


#include "gtest/gtest.h"
#include<bits/stdc++.h>
#include <scalable/scalable.h>

using namespace scalable;


TEST(collect, map_filter_reduce_should_perform_arithmetics_on_doubles_vector) {
	std::vector<int> arr{1,2,3,4,5};
	auto v = collect(arr)
				.map( fun([] (int i ) {return i * i;}))
				.filter( fun([] (int i ) {return i > 5;}))
				.reduce(fun([] (int i, int j) {return i * j;}));
	int i = (9 * 16 * 25);
	EXPECT_EQ(v, i);


}
TEST(collect, map_filter_reduce_should_perform_arithmetics_on_int_set) {

	std::set<int> myset{1,2,3,4,5};
	auto v1 =  collect(myset)
					.map(fun([] (int i ) {return i *2; }))
					.filter(fun([](int j) {return j < 8;}))
					.reduce(fun([](int j, int i) {return i + j;}));
	EXPECT_DOUBLE_EQ(v1, 12);
}
TEST(collect, flatMap_should_work) {

		auto b = collect(std::vector<int>{1,2,3,4})
				.flatMap(fun([] (int j) {
					std::vector<int> v;
					for(int i =0; i < j;++i) v.push_back(i);
					return v;
				}));
		std::vector<int> result{0,0,1,0,1,2,0,1,2,3};
		EXPECT_EQ(b.get().size(), result.size());
		for(unsigned i =0; i < std::min(b.get().size(), result.size()); ++i)
			EXPECT_EQ((b.get()[i]), (result[i]));

}

TEST(collectMap, map_should_produce_result_of_function_with_key) {

	std::map<std::string, double> mymap{{"A",1},{"B",2},{"C",3}};
	auto m = collectMap(mymap)
		.map(fun ( [] (std::string s, double d) {
		std::stringstream ss;
		ss << s << "-"<<d;
		return ss.str();
	}));
	EXPECT_EQ(m.get().at("A"), "A-1");
	EXPECT_EQ(m.get().at("B"), "B-2");
	EXPECT_EQ(m.get().at("C"), "C-3");
  

}

TEST(collect, vector_of_pairs_should_be_convertible_to_tree_map) {
    auto m1 =  collect(std::vector<std::pair<int,int>>{{1,2},{3,4}})
        .toTreeMap().get();

    EXPECT_EQ(m1.at(1), 2);
    EXPECT_EQ(m1.at(3), 4);
}

TEST(collect, set_of_pairs_should_be_convertible_to_hash_map) {
    std::unordered_map<int,int> m2=collect(std::set<std::pair<int,int>>{{7,2},{6,4}})
        .toHashMap().get();

    EXPECT_EQ(m2.at(7), 2);
    EXPECT_EQ(m2.at(6), 4);
}

TEST(collect, collect_should_group_numbers_by_parity_preserving_order) {
    auto m1 = collect(std::vector<int>{1, 2, 3, 4, 5, 6, 7 ,8})
        .groupBy(fun([](int i) {return i % 2 == 0;})).get();
        
     EXPECT_EQ(m1.at(true), std::list<int>({2,4,6,8}));
     EXPECT_EQ(m1.at(false), std::list<int>({1,3,5,7}));
}

TEST(collect, scanLeft_should_compute_cumsum_of_squares) {
    std::vector<int> data{1, 2, 3, 4, 5, 6, 7 ,8};
    auto m = collect(data)
        .scanLeft(0, fun([](int i, int j) { return i + j * j;}))
        .scanLeft(std::make_pair(0,0), fun([](std::pair<int,int> i, int j) { 
                return std::make_pair(i.second, j);
        }))
        .map(fun([](std::pair<int, int> p) { 
            return p.second - p.first; 
        }))
        .get();
        
        
    auto squares = collect(data).map(fun([] ( int i ) {
        return i * i;
    })).get();
    EXPECT_EQ(m, squares);
}

void function_for_template_compilation_tests() {
    is_pair<std::pair<int, int>>::map_type map1;
    is_pair<int> stru;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

