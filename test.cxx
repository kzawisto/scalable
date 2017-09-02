


#include "gtest/gtest.h"
#include<bits/stdc++.h>
#include <scalable/scalable.h>

using namespace scalable;
TEST(collect, map_filter_reduce_should_perform_arithmetics_on_doubles) {
	std::vector<int> arr{1,2,3,4,5};
	auto v = collect(arr)
				.map( fun([] (int i ) {return i * i;}))
				.filter( fun([] (int i ) {return i > 5;}))
				.reduce(fun([] (int i, int j) {return i * j;}));
	int i = (9 * 16 * 25);
	EXPECT_EQ(v, i);


}
TEST(collect, map_filter_reduce_should_work_for_set) {

	std::set<double> myset{1,2,3,4,5};
	auto v1 =  collect(myset)
					.map(fun([] (double i ) {return i *2; }))
					.filter(fun([](double j) {return j < 8;}))
					.reduce(fun([](double j, double i) {return i + j;}));
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

TEST(collectMap, should_work_for_map) {

	std::map<std::string, double> mymap{{"A",1},{"B",2},{"C",3}};
	auto v1 =  collectMap(mymap);
    collectMap(mymap).toVec();
    collectMap(mymap).toTreeSet();
	auto m = collectMap(mymap)
		.map(fun ( [] (std::string s, double d) {
		std::stringstream ss;
		ss << s << "-"<<d;
		return ss.str();
	}));
	EXPECT_EQ(m.get().at("A"), "A-1");
	EXPECT_EQ(m.get().at("B"), "B-2");
	EXPECT_EQ(m.get().at("C"), "C-3");
  
	is_pair<std::pair<int, int>>::map_type map1;
    is_pair<int> stru;
    auto m1 =  collect(std::vector<std::pair<int,int>>{{1,2},{3,4}})
        .toTreeMap().get();

    EXPECT_EQ(m1.at(1), 2);
    EXPECT_EQ(m1.at(3), 4);
    std::unordered_map<int,int> m2=collect(std::vector<std::pair<int,int>>{{7,2},{6,4}})
        .toHashMap().get();

    EXPECT_EQ(m2.at(7), 2);
    EXPECT_EQ(m2.at(6), 4);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

