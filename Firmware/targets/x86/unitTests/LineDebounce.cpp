#include "LineDebounce.h"
#include "gtest/gtest.h"

using libs::debouncer::LineDebounce;
using libs::debouncer::State;

TEST(LineDebounce, initial) {
	LineDebounce<5> debounce;
	EXPECT_EQ(false, debounce.getValue());
	EXPECT_EQ(State::UNSTABLE, debounce.getState());

	for(int i = 0 ; i < 4 ; i++){
		debounce.tick(false);
		EXPECT_EQ(false, debounce.getValue());
		EXPECT_EQ(State::UNSTABLE, debounce.getState());
	}

	debounce.tick(false);
	EXPECT_EQ(false, debounce.getValue());
	EXPECT_EQ(State::STABLE, debounce.getState());
}

template <uint32_t debounceTime>
void InitializeDebouncerToStableFalse(LineDebounce<debounceTime> & debounce){
	for(uint32_t i = 0 ; i < debounceTime ; i++){
		debounce.tick(false);
	}
	EXPECT_EQ(false, debounce.getValue());
	EXPECT_EQ(State::STABLE, debounce.getState());
}

TEST(LineDebounce, basic) {
	LineDebounce<5> debounce;

	InitializeDebouncerToStableFalse(debounce);

	debounce.tick(true);
	EXPECT_EQ(false, debounce.getValue());
	EXPECT_EQ(State::UNSTABLE, debounce.getState());

	for(int i = 0 ; i < 5 ; i++){
		debounce.tick(false);
		EXPECT_EQ(false, debounce.getValue());
		EXPECT_EQ(State::UNSTABLE, debounce.getState());
	}

	debounce.tick(false);
	EXPECT_EQ(false, debounce.getValue());
	EXPECT_EQ(State::STABLE, debounce.getState());

	for(int i = 0 ; i < 5 ; i++){
		debounce.tick(true);
		EXPECT_EQ(false, debounce.getValue());
		EXPECT_EQ(State::UNSTABLE, debounce.getState());
	}

	debounce.tick(true);
	EXPECT_EQ(true, debounce.getValue());
	EXPECT_EQ(State::TRANSITION, debounce.getState());

	debounce.tick(true);
	EXPECT_EQ(true, debounce.getValue());
	EXPECT_EQ(State::STABLE, debounce.getState());
}

TEST(LineDebounce, criticalFrequencyApplied) {
	LineDebounce<5> debounce;

	InitializeDebouncerToStableFalse(debounce);

	for(int j = 0 ; j < 5 ; j ++){
		for(int i = 0 ; i < 5 ; i++){
			debounce.tick(true);
			EXPECT_EQ(false, debounce.getValue());
			EXPECT_EQ(State::UNSTABLE, debounce.getState());
		}

		debounce.tick(true);
		EXPECT_EQ(true, debounce.getValue());
		EXPECT_EQ(State::TRANSITION, debounce.getState());

		for(int i = 0 ; i < 5 ; i++){
			debounce.tick(false);
			EXPECT_EQ(true, debounce.getValue());
			EXPECT_EQ(State::UNSTABLE, debounce.getState());
		}

		debounce.tick(false);
		EXPECT_EQ(false, debounce.getValue());
		EXPECT_EQ(State::TRANSITION, debounce.getState());
	}

	debounce.tick(false);
	EXPECT_EQ(false, debounce.getValue());
	EXPECT_EQ(State::STABLE, debounce.getState());
}
