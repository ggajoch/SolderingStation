#include <array>
#include "gtest/gtest.h"

#include "CLI.h"
#include "HALmock.h"

namespace libs {
namespace CLI {
extern libs::array_view<Command*> commands;
}
}
using libs::CLI::commands;
using libs::CLI::set_commands;
using libs::CLI::Command;

struct CLITest_;

std::vector<CLITest_*> testCommands;

std::array<char*, 50> params;
libs::array_view<char*> rx_params;

struct CLITest_ : Command {
    explicit CLITest_(const char* name) : Command(name), callbacked(false) {
        testCommands.push_back(this);
    }

    CLITest_(const char* name, int requiredArguments) : Command(name, requiredArguments), callbacked(false) {
        testCommands.push_back(this);
    }

    void callback(const libs::array_view<char*> parameters) override {
        // printf("callback: %s | ", this->name);
        // for(auto x : parameters) {
        // printf("%p, %s, ", x, x);
        // }
        // printf("\n");

        rx_params = parameters;
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
    CLITest1_() : CLITest_("test1") {
    }
};

struct CLITest2_ : CLITest_ {
    CLITest2_() : CLITest_("test2") {
    }
};

struct CLITest3_ : CLITest_ {
    CLITest3_() : CLITest_("test") {
    }
};

struct CLITest4_ : CLITest_ {
    CLITest4_() : CLITest_("test21") {
    }
};

struct CLITest5_ : CLITest_ {
    CLITest5_() : CLITest_("test5", 3) {
    }
};

CLITest1_* CLITest1;
CLITest2_* CLITest2;
CLITest3_* CLITest3;
CLITest4_* CLITest4;
CLITest5_* CLITest5;

class CLITest : public ::testing::Test {
    void SetUp() {
        static CLITest1_ CLITest1_o;
        static CLITest2_ CLITest2_o;
        static CLITest3_ CLITest3_o;
        static CLITest4_ CLITest4_o;
        static CLITest5_ CLITest5_o;

        CLITest1 = &CLITest1_o;
        CLITest2 = &CLITest2_o;
        CLITest3 = &CLITest3_o;
        CLITest4 = &CLITest4_o;
        CLITest5 = &CLITest5_o;

        static Command* tab[] = {CLITest1, CLITest2, CLITest3, CLITest4, CLITest5};
        auto x = libs::array_view<Command*>(tab);
        set_commands(x);
    }
};

TEST_F(CLITest, init) {
    EXPECT_EQ(commands.size(), 5);
    EXPECT_EQ(testCommands.size(), 5);

    for (auto x : testCommands) {
        EXPECT_FALSE(x->callbacked);
    }
}

static void parse(const char* cmd) {
    static char tmp[100];
    strcpy(tmp, cmd);
    libs::CLI::parse_line(tmp);
}

void checkIfCallbacked(Command* cmd) {
    bool was = false;
    for (auto x : testCommands) {
        if (x != cmd) {
            EXPECT_FALSE(x->wasCallbacked());
        } else {
            EXPECT_TRUE(x->wasCallbacked());
            was = true;
        }
    }
    if (cmd != nullptr)
        EXPECT_TRUE(was);
}

int actualTesting;
template <typename T>
void testX(int len, T first) {
    EXPECT_STREQ(rx_params[actualTesting++], first);
}

template <typename T, typename... Args>
void testX(int len, T first, Args... args) {
    EXPECT_STREQ(rx_params[actualTesting++], first);
    testX(len, args...);
}

void test() {
    EXPECT_EQ(rx_params.size(), 0);
}

template <typename T>
void test(T v) {
    EXPECT_EQ(rx_params.size(), 1);
    EXPECT_STREQ(rx_params[0], v);
}

template <typename T, typename... Args>
void test(T first, Args... args) {
    int nArg = sizeof...(args) + 1;
    actualTesting = 0;

    EXPECT_EQ(rx_params.size(), nArg);
    testX(nArg, first, args...);
    EXPECT_EQ(actualTesting, nArg);
}

void testVec(std::vector<char*> params) {
    EXPECT_EQ(params.size(), rx_params.size());
    for (int i = 0; i < params.size(); ++i) {
        EXPECT_STREQ(rx_params[i], params[i]);
    }
}

void gen_random_string(char* s, const int len) {
    static const char alphanum[] = "0123456789"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

TEST_F(CLITest, simple1) {
    parse("test1 1 2 3");
    checkIfCallbacked(CLITest1);
    test("1", "2", "3");

    parse("test2");
    checkIfCallbacked(CLITest2);
    test();
}

TEST_F(CLITest, incorrectNrOfArugments) {
    parse("test1 1 2 3");
    checkIfCallbacked(CLITest1);
    test("1", "2", "3");

    parse("test5");
    checkIfCallbacked(nullptr);

    // EXPECT_TRUE(HAL::Com::checkLastLine("ERR Required 3 arguments for test5\n"));

    parse("test5 1");
    checkIfCallbacked(nullptr);
    // EXPECT_TRUE(HAL::Com::checkLastLine("ERR Required 3 arguments for test5\n"));

    parse("test5 1 2");
    checkIfCallbacked(nullptr);
    // EXPECT_TRUE(HAL::Com::checkLastLine("ERR Required 3 arguments for test5\n"));

    parse("test5 1 2 3 ");
    checkIfCallbacked(CLITest5);
    // EXPECT_TRUE(HAL::Com::checkLastLine(""));

    parse("test5 1 2 3 4 ");
    checkIfCallbacked(nullptr);
    // EXPECT_TRUE(HAL::Com::checkLastLine("ERR Required 3 arguments for test5\n"));
}

TEST_F(CLITest, allCasesRandom) {
    std::srand(std::time(0));

    for (int cases = 0; cases < 100; ++cases) {
        for (auto test : testCommands) {
            for (int params = 0; params < 20; ++params) {
                int nowParams = params;
                if (test->requiredArguments != 1001) {
                    nowParams = test->requiredArguments;
                }
                static char buf[100];
                char* bufPtr = buf;
                bufPtr += sprintf(bufPtr, "%s ", test->name);

                std::vector<char*> actualParameters;

                for (int i = 0; i < nowParams; ++i) {
                    int len = 1 + std::rand() % 5;
                    gen_random_string(bufPtr, len);
                    // printf("[%d] = %s\n", i, bufPtr);
                    actualParameters.push_back(bufPtr);
                    bufPtr += len;
                    *bufPtr = ' ';
                    ++bufPtr;
                }
                *bufPtr = '\0';

                // printf("Test: |%s|\n", buf);

                parse(buf);
                checkIfCallbacked(test);
                for (char* bufTmp = buf; bufTmp < bufPtr; ++bufTmp) {
                    if (*bufTmp == ' ') {
                        *bufTmp = '\0';
                    }
                }
                testVec(actualParameters);
            }
        }
    }
}
