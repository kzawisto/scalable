


#include "gtest/gtest.h"
#include<bits/stdc++.h>
#include "scalable.h"

TEST(span, map_filter_reduce_should_work_for_vector) {
	std::vector<int> arr{1,2,3,4,5};
	auto v = span(arr)
				.map( fun([] (int i ) {return i * i;}))
				.filter( fun([] (int i ) {return i > 5;}))
				.reduce(fun([] (int i, int j) {return i * j;}));
	int i = (9 * 16 * 25);
	EXPECT_EQ(v, i);


}
TEST(span, map_filter_reduce_should_work_for_set) {

	std::set<double> myset{1,2,3,4,5};
	auto v1 =  span(myset)
					.map(fun([] (double i ) {return i *2; }))
					.filter(fun([](double j) {return j < 8;}))
					.reduce(fun([](double j, double i) {return i + j;}));
	EXPECT_DOUBLE_EQ(v1, 12);
}
TEST(span, flatMap_should_work) {

		auto b = span(std::vector<int>{1,2,3,4})
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

TEST(spanMap, should_work_for_map) {

	std::map<std::string, double> mymap{{"A",1},{"B",2},{"C",3}};
	auto v1 =  spanMap(mymap);
	auto m = spanMap(mymap)
		.map(fun ( [] (std::string s, double d) {
		std::stringstream ss;
		ss << s << "-"<<d;
		return ss.str();
	}));
	EXPECT_EQ(m.get().at("A"), "A-1");
	EXPECT_EQ(m.get().at("B"), "B-2");
	EXPECT_EQ(m.get().at("C"), "C-3");


}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
