#include <gtest/gtest.h>
#include "HALmock.h"

#include "stateManager.h"

#include "storage/storage.h"
#include "storage/settings.h"
#include "storage/state.h"
#include "storage/layout.h"

void clear_memory() {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0xFF);
}

bool memory_empty() {
    for(auto x : HAL::Memory::table) {
        if (x != 0xFF) {
            return false;
        }
    }
    return true;
}

TEST(State, readCorrectValue) {
    i2cMemoryStateLayout state;
    state.marker = 0x00;
    state.temperature = 15689;
    state.crc = state.calculate_crc();

    clear_memory();
    HAL::Memory::table[32+10*4+0] = state.as_span()[0];
    HAL::Memory::table[32+10*4+1] = state.as_span()[1];
    HAL::Memory::table[32+10*4+2] = state.as_span()[2];
    HAL::Memory::table[32+10*4+3] = state.as_span()[3];

    core::setup();

    EXPECT_EQ(core::persistent_state.target, 15689);
}

TEST(State, readIncorrectMarker) {
    i2cMemoryStateLayout state;
    state.marker = 0x01;
    state.temperature = 15689;
    state.crc = state.calculate_crc();


    clear_memory();
    HAL::Memory::table[32+10*4+0] = state.as_span()[0];
    HAL::Memory::table[32+10*4+1] = state.as_span()[1];
    HAL::Memory::table[32+10*4+2] = state.as_span()[2];
    HAL::Memory::table[32+10*4+3] = state.as_span()[3];

    core::persistent_state.target = 10;

    core::setup();

    EXPECT_EQ(core::persistent_state.target, 0);
}

TEST(State, readIncorrectCRC) {
    i2cMemoryStateLayout state;
    state.marker = 0x00;
    state.temperature = 15689;
    state.crc = state.calculate_crc()+1;


    clear_memory();
    HAL::Memory::table[32+10*4+0] = state.as_span()[0];
    HAL::Memory::table[32+10*4+1] = state.as_span()[1];
    HAL::Memory::table[32+10*4+2] = state.as_span()[2];
    HAL::Memory::table[32+10*4+3] = state.as_span()[3];

    core::persistent_state.target = 10;

    core::setup();

    EXPECT_EQ(core::persistent_state.target, 0);
}

TEST(State, saveAfter100Ticks) {
    for(int i = 0; i < 200; ++i) {
        core::tick();
    }

    clear_memory();
    EXPECT_TRUE(core::storage::stateIsSaved());
    EXPECT_EQ(HAL::Display::line1[8], ' ');
    EXPECT_TRUE(memory_empty());

    core::persistent_state.target = 10;

    for(int i = 0; i < 100; ++i) {
        core::tick();
        EXPECT_FALSE(core::storage::stateIsSaved());
        EXPECT_EQ(HAL::Display::line1[8], '*');
        EXPECT_TRUE(memory_empty());
    }

    core::tick();
    EXPECT_EQ(HAL::Display::line1[8], ' ');
    EXPECT_TRUE(core::storage::stateIsSaved());
    EXPECT_FALSE(memory_empty());
}

TEST(State, saveAfterMultiChange) {
    for(int i = 0; i < 200; ++i) {
        core::tick();
    }

    clear_memory();
    EXPECT_TRUE(core::storage::stateIsSaved());
    EXPECT_EQ(HAL::Display::line1[8], ' ');
    EXPECT_TRUE(memory_empty());

    for(int changes = 0; changes < 10; ++changes) {
        core::persistent_state.target = 147+changes;

        for (int i = 0; i < 100; ++i) {
            core::tick();
            EXPECT_FALSE(core::storage::stateIsSaved());
            EXPECT_EQ(HAL::Display::line1[8], '*');
            EXPECT_TRUE(memory_empty());
        }
    }

    core::tick();
    EXPECT_EQ(HAL::Display::line1[8], ' ');
    EXPECT_TRUE(core::storage::stateIsSaved());
    EXPECT_FALSE(memory_empty());
}

void save(uint16_t value) {
    core::persistent_state.target = 915;
    core::tick();
    core::persistent_state.target = value;
    ASSERT_LT(value, 1000);

    for(int i = 0; i < 100; ++i) {
        core::tick();
        EXPECT_FALSE(core::storage::stateIsSaved());
        EXPECT_EQ(HAL::Display::line1[8], '*');
    }
    core::tick();
    EXPECT_EQ(HAL::Display::line1[8], ' ');
    EXPECT_TRUE(core::storage::stateIsSaved());
}

static gsl::span<i2cMemoryStateLayout> ptr{reinterpret_cast<i2cMemoryStateLayout*>(HAL::Memory::table.data()+32),
                                           (static_cast<uint16_t>(core::config::memory_type)-32)/4};

void check(uint16_t slot, uint8_t marker, uint16_t temperature, uint8_t crc) {
    EXPECT_EQ(ptr[slot].marker, marker);
    EXPECT_EQ(ptr[slot].temperature, temperature);
    EXPECT_EQ(ptr[slot].crc, crc);
}

TEST(State, whenMemoryEmptyShouldSaveToLowestSlot) {
    clear_memory();

    save(0);

    EXPECT_EQ(HAL::Memory::table[32], 0);
    EXPECT_EQ(HAL::Memory::table[33], 0);
    EXPECT_EQ(HAL::Memory::table[34], 0);
    EXPECT_EQ(HAL::Memory::table[35], 0);

    check(0, 0, 0, 0);
}


TEST(State, clearingCell) {
    clear_memory();
    check(0, 0xFF, 0xFFFF, 0xFF);
    check(1, 0xFF, 0xFFFF, 0xFF);
    check(2, 0xFF, 0xFFFF, 0xFF);

    save(0);
    check(0, 0, 0, 0);
    check(1, 0xFF, 0xFFFF, 0xFF);
    check(2, 0xFF, 0xFFFF, 0xFF);

    save(0);
    check(0, 0xFF, 0, 0);
    check(1, 0, 0, 0);
    check(2, 0xFF, 0xFFFF, 0xFF);

    save(0);
    check(0, 0xFF, 0, 0);
    check(1, 0xFF, 0, 0);
    check(2, 0, 0, 0);

    save(785);
    check(0, 0xFF, 0, 0);
    check(1, 0xFF, 0, 0);
    check(2, 0xFF, 0, 0);
    EXPECT_EQ(ptr[3].marker, 0);
    EXPECT_EQ(ptr[3].temperature, 785);
    EXPECT_NE(ptr[3].crc, 0);
}

TEST(State, checkAllSlotsMarker) {
    clear_memory();
    for(auto slot: ptr) {
        EXPECT_EQ(slot.marker, 0xFF);
        EXPECT_EQ(slot.temperature, 0xFFFF);
        EXPECT_EQ(slot.crc, 0xFF);
    }

    auto first_pass = [](uint16_t index) {
        return index % 547 + 157;
    };
    auto second_pass = [](uint16_t index) {
        return index % 547 + 21;
    };

    // first pass through memory, cells after current are empty
    for(uint16_t i = 0; i < ptr.size(); ++i) {
        save(first_pass(i));

        for(int slot = 0; slot < i; ++slot) {
            EXPECT_EQ(ptr[slot].marker, 0xFF);
            EXPECT_EQ(ptr[slot].temperature, first_pass(slot));
        }

        EXPECT_EQ(ptr[i].marker, 0);
        EXPECT_EQ(ptr[i].temperature, first_pass(i));

        for(int slot = i+1; slot < ptr.size(); ++slot) {
            EXPECT_EQ(ptr[slot].marker, 0xFF);
            EXPECT_EQ(ptr[slot].temperature, 0xFFFF);
            EXPECT_EQ(ptr[slot].crc, 0xFF);
        }

        // check readout of correct cell
        core::persistent_state.target = i;
        core::setup();
        EXPECT_EQ(core::persistent_state.target, first_pass(i));
    }

    // second pass through memory, only markers are empty
    for(uint16_t i = 0; i < ptr.size(); ++i) {
        save(second_pass(i));

        for(int slot = 0; slot < i; ++slot) {
            EXPECT_EQ(ptr[slot].marker, 0xFF);
            EXPECT_EQ(ptr[slot].temperature, second_pass(slot));
        }

        EXPECT_EQ(ptr[i].marker, 0);
        EXPECT_EQ(ptr[i].temperature, second_pass(i));

        for(int slot = i+1; slot < ptr.size(); ++slot) {
            EXPECT_EQ(ptr[slot].marker, 0xFF);
            EXPECT_EQ(ptr[slot].temperature, first_pass(slot));
        }

        // check readout of correct cell
        core::persistent_state.target = i;
        core::setup();
        EXPECT_EQ(core::persistent_state.target, second_pass(i));
    }
}