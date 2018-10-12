


#include "gtest/gtest.h"
#include<bits/stdc++.h>
#include <scalable/scalable.h>
using namespace scalable;


TEST(collect, map_filter_reduce_should_perform_arithmetics_on_doubles_vector) {
	std::vector<int> arr{1,2,3,4,5};
	auto v = collect(arr)
				.map( [] (int i ) {return i * i;})
				.filter( fun([] (int i ) {return i > 5;}))
				.reduce(fun([] (int i, int j) {return i * j;}));
	int i = (9 * 16 * 25);
	EXPECT_EQ(v, i);


}

TEST(collect, map_filter_reduce_should_perform_arithmetics_on_int_set) {

	std::set<int> myset{1,2,3,4,5};
	auto v1 =  collect(myset)
					.map([] (int i ) {return i *2; })
					.filter([](const int &j) {return j < 8;})
					.reduce([](int j, int i) {return i + j;});
	EXPECT_DOUBLE_EQ(v1, 12);
}
TEST(collect, flatMap_should_work) {

		auto b = collect(std::vector<int>{1,2,3,4})
				.flatMap([] (const int & j) {
					std::vector<int> v;
					for(int i =0; i < j;++i) v.push_back(i);
					return v;
				});
		std::vector<int> result{0,0,1,0,1,2,0,1,2,3};
		EXPECT_EQ(b.get().size(), result.size());
		for(unsigned i =0; i < std::min(b.get().size(), result.size()); ++i)
			EXPECT_EQ((b.get()[i]), (result[i]));

}

TEST(collectMap, map_should_produce_result_of_function_with_key) {

	std::map<std::string, double> mymap{{"A",1},{"B",2},{"C",3}};
	auto m = collectMap(mymap)
		.map( [] (std::string s, double d) {
		std::stringstream ss;
		ss << s << "-"<<d;
		return ss.str();
	});

	EXPECT_EQ(m.get().at("A"), "A-1");
	EXPECT_EQ(m.get().at("B"), "B-2");
	EXPECT_EQ(m.get().at("C"), "C-3");
  

}
TEST(collectMap, map_should_filter_by_keys) {

	std::map<std::string, double> mymap{{"A",1},{"B",2},{"C",3}};
	auto m = collectMap(mymap)
		.filter( [] (std::string s, double d) {
		return s != "A";

	});

	std::set<std::string> expected {"B","C"};
	EXPECT_EQ(m.getKeys().get(), expected);

}
TEST(collectMap, map_should_filter_by_vals) {

	std::map<std::string, double> mymap{{"A",1},{"B",2},{"C",3}};
	auto m = collectMap(mymap)
		.filter( [] (std::string s, double d) {
		return d != 2;

	});
	std::set<std::string> expected {"A","C"};
	EXPECT_EQ(m.getKeys().get(), expected);

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

TEST(collect, mapping_should_work_on_hash_map) {
    std::unordered_map<int,double> m2=collect(std::set<std::pair<int,int>>{{7,2},{6,4}})
        .toHashMap().map([] ( int i, int j ){return j +1.0;}).get();

    EXPECT_EQ(m2.at(7), 3);
    EXPECT_EQ(m2.at(6), 5);
}


TEST(collect, collect_should_group_numbers_by_parity_preserving_order) {
    auto m1 = collect(std::vector<int>{1, 2, 3, 4, 5, 6, 7 ,8})
        .groupBy([](int i) {return i % 2 == 0;}).get();
        
     EXPECT_EQ(m1.at(true), std::vector<int>({2,4,6,8}));
     EXPECT_EQ(m1.at(false), std::vector<int>({1,3,5,7}));
}


TEST(collect, scanLeft_should_compute_cumsum_of_squares) {
    std::vector<int> data{1, 2, 3, 4, 5, 6, 7 ,8};
    auto m = collect(data)
        .scanLeft(0, [](int i, int j) { return i + j * j;})
        .scanLeft(std::make_pair(0,0), fun([](std::pair<int,int> i, int j) { 
                return std::make_pair(i.second, j);
        }))
        .map([](std::pair<int, int> p) {
            return p.second - p.first; 
        })
        .get();
        
        
    auto squares = collect(data).map(fun([] ( int i ) {
        return i * i;
    })).get();
    EXPECT_EQ(m, squares);
}


TEST(collectMap, get_key_or_values_from_map){ 
    auto col = collectMap(std::map<std::string, int> {{"A", 1},{"B", 2}});
    auto expectedKeys = std::set<std::string>{"A","B"};
    auto expectedValues = std::vector<int>{1,2};
    EXPECT_EQ(col.getKeys().get(),expectedKeys);
    EXPECT_EQ(col.getValues().get(),expectedValues);

}

TEST(collect, showcase_example) {
	collect(std::set<int>{1,2,3,4,5})
					.map([](int i ) {
                                return i * i; 
                    })
                    .filter([](int j) {
                                return j < 18;
                    })
                    .flatMap([](int i) {
                                return std::set<int>{ i, i + 1, i + 2};
                    })
                    .groupBy([](int j) {
                                return j % 2 == 0;
                    })
                    .map([](bool v, std::vector<int> l) {
                                    return collect(l)
                                        .map([](int i) {
                                                return std::to_string(i);
                                        })
                                        .reduce(fun([] (std::string lhs, std::string rhs){
                                                return lhs + "_" + rhs;
                                                    }));
                                })
                    .get()
                    .at(true);
}

TEST(collect, convert_data_to_vector) {
	int arr [] {1,2,3,4,5};
	vectorize(arr, arr + 5).map([] (int i ) {return i * 2;}).get();
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

