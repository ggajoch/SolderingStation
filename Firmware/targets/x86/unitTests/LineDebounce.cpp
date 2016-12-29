#include "LineDebounce.h"
#include "gtest/gtest.h"

using libs::debouncer::LineDebounce;
using libs::debouncer::State;

TEST(LineDebounceTest, basic) {
	LineDebounce<5> debounce;
	EXPECT_EQ(false, debounce.getValue());
	EXPECT_EQ(State::UNSTABLE, debounce.getState());

	for(int i = 0 ; i < 4 ; i++){
		debounce.tick(false);
		EXPECT_EQ(false, debounce.getValue());
		EXPECT_EQ(State::UNSTABLE, debounce.getState());
	}

}
