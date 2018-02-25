#include <array>
#include "gtest/gtest.h"

#include "CLI.h"
#include "HALmock.h"
#include "core.h"

using libs::CLI::set_commands;
using libs::CLI::Command;

struct CLITest_;

std::array<int, 100> params_buf;
gsl::span<int> params;


struct CLITest_ : Command {
    CLITest_(const char* name, unsigned int requiredArguments) : Command(name, requiredArguments), callbacked(false) {
    }

    void callback(const gsl::span<char*> parameters) override {
        for(int i = 0; i < parameters.size(); ++i) {
            params_buf[i] = atoi(parameters[i]);
        }
        params = gsl::span<int>(params_buf.begin(), parameters.size());
        callbacked = true;
    }

    bool wasCallbacked() {
        bool temp = callbacked;
        callbacked = false;
        return temp;
    }

    bool callbacked;
};

struct CLITest1_ : CLITest_ {
    CLITest1_() : CLITest_("test1", 1) {
    }
};

struct CLITest2_ : CLITest_ {
    CLITest2_() : CLITest_("test2", 2) {
    }
};

struct CLITest3_ : CLITest_ {
    CLITest3_() : CLITest_("test", 0) {
    }
};

struct CLITest4_ : CLITest_ {
    CLITest4_() : CLITest_("test21", 3) {
    }
};

struct CLITest5_ : CLITest_ {
    CLITest5_() : CLITest_("test5", 5) {
    }
};

std::array<Command*, 5> commands;

CLITest1_ CLITest1;
CLITest2_ CLITest2;
CLITest3_ CLITest3;
CLITest4_ CLITest4;
CLITest5_ CLITest5;

void init() {
    commands[0] = &CLITest1;
    commands[1] = &CLITest2;
    commands[2] = &CLITest3;
    commands[3] = &CLITest4;
    commands[4] = &CLITest5;
}

class CLITest : public ::testing::Test {
 public:
    std::vector<CLITest_*> testCommands;

    void SetUp() {
        init();

        testCommands.push_back(&CLITest1);
        testCommands.push_back(&CLITest2);
        testCommands.push_back(&CLITest3);
        testCommands.push_back(&CLITest4);
        testCommands.push_back(&CLITest5);
        
        set_commands(commands);
    }

    void checkIfCallbacked(Command* cmd = nullptr) {
        for (auto x : testCommands) {
            EXPECT_EQ(x == cmd, x->wasCallbacked());
        }
    }

    void parse(const char* cmd) {
        static char tmp[100];
        strcpy(tmp, cmd);
        HAL::Com::handler(tmp);
        checkIfCallbacked(nullptr); // nothing is invoked until tick()
        core::tick();
    }

    void check_params(std::initializer_list<int> p) {
        EXPECT_EQ(p.size(), params.size());
        EXPECT_TRUE(std::equal(p.begin(), p.end(), params.begin()));
    }
};

TEST_F(CLITest, simpleOK) {
    parse("test1 5");
    checkIfCallbacked(&CLITest1);
    check_params({5});

    parse("test2 1 2");
    checkIfCallbacked(&CLITest2);
    check_params({1, 2});
}

TEST_F(CLITest, noCommand) {
    parse("teeeee 5");
    checkIfCallbacked();

    parse("tester 1 2");
    checkIfCallbacked();
}

TEST_F(CLITest, incorrectNrOfArugments) {
    parse("test1");
    checkIfCallbacked();

    parse("test2 1 3 2");
    checkIfCallbacked();


    parse("test21 1  ");
    checkIfCallbacked();

    parse("test 1");
    checkIfCallbacked();

    parse("test5 1 2 2 4");
    checkIfCallbacked();
}

TEST_F(CLITest, whitespace) {
    parse("test");
    checkIfCallbacked(&CLITest3);
    check_params({});

    parse("test  ");
    checkIfCallbacked(&CLITest3);
    check_params({});
    parse("test   ");
    checkIfCallbacked(&CLITest3);
    check_params({});

    parse("test2   1   3 ");
    checkIfCallbacked(&CLITest2);
    check_params({1, 3});
}
