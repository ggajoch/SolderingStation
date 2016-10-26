#include "gtest/gtest.h"

#include "CLI.h"

using libs::CLI::commands;
using libs::CLI::nrOfCommands;
using libs::CLI::Command;

struct CLITest_;

std::vector<CLITest_ *> testCommands;

std::array<char*, 20> params;
libs::array_view<char *> rx_params;

struct CLITest_ : Command {
    explicit CLITest_(const char *name) : Command(name), callbacked(false) {
        testCommands.push_back(this);
    }

    void callback(const libs::array_view<char *> parameters) override {
//        printf("callback: %s | ", this->name);
//        for(auto x : parameters) {
//            printf("%p, %s, ", x, x);
//        }
//        printf("\n");

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
    CLITest1_() : CLITest_("test1") {}
} CLITest1;

struct CLITest2_ : CLITest_ {
    CLITest2_() : CLITest_("test2") {}
} CLITest2;

struct CLITest3_ : CLITest_ {
    CLITest3_() : CLITest_("test") {}
} CLITest3;

struct CLITest4_ : CLITest_ {
    CLITest4_() : CLITest_("test21") {}
} CLITest4;



TEST(CLI, init) {
    EXPECT_EQ(nrOfCommands, 4);
    EXPECT_EQ(testCommands.size() , nrOfCommands);

    for (auto x : testCommands) {
        EXPECT_FALSE(x->callbacked);
    }
}

void parse(const char * cmd) {
    static char tmp[100];
    strcpy(tmp, cmd);
    libs::CLI::parse_line(tmp);
}

void checkIfCallbacked(Command * cmd) {
    for (auto x : testCommands) {
        if (x != cmd) {
            EXPECT_FALSE(x->wasCallbacked());
        } else {
            EXPECT_TRUE(x->wasCallbacked());
        }
    }
}


int actualTesting;
template<typename T>
void testX(int len, T first) {
    EXPECT_STREQ(rx_params[actualTesting++], first);
}

template<typename T, typename... Args>
void testX(int len, T first, Args... args) {
    EXPECT_STREQ(rx_params[actualTesting++], first);
    testX(len, args...);
}


void test() {
    EXPECT_EQ(rx_params.size(), 0);
}

template<typename T>
void test(T v) {
    EXPECT_EQ(rx_params.size(), 1);
    EXPECT_STREQ(rx_params[0], v);
}

template<typename T, typename... Args>
void test(T first, Args... args) {
    int nArg = sizeof...(args)+1;
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


void gen_random_string(char *s, const int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

TEST(CLI, simple1) {
    parse("test1 1 2 3");
    checkIfCallbacked(&CLITest1);
    test("1", "2", "3");

    parse("test2");
    checkIfCallbacked(&CLITest2);
    test();
}

TEST(CLI, allCasesRandom) {
    std::srand(std::time(0));

    for (int params = 0; params < 20; ++params) {
        for (int cases = 0; cases < 100; ++cases) {
            for (auto test : testCommands) {
                static char buf[100];
                char * bufPtr = buf;
                bufPtr += sprintf(bufPtr, "%s ", test->name);

                std::vector<char*> actualParameters;

                for (int i = 0; i < params; ++i) {
                    int len = 1+std::rand() % 5;
                    gen_random_string(bufPtr, len);
                    actualParameters.push_back(bufPtr);
                    bufPtr += len;
                    *bufPtr = ' ';
                    ++bufPtr;
                }
                *bufPtr = '\0';

//                printf("Test: |%s|\n", buf);

                parse(buf);
                checkIfCallbacked(test);
                for (char * bufTmp = buf; bufTmp < bufPtr; ++bufTmp) {
                    if (*bufTmp == ' ') {
                        *bufTmp = '\0';
                    }
                }
                testVec(actualParameters);
            }
        }
    }
}
