#pragma once

#include <functional>
#include <iostream>
#include <sstream>

void register_test_function(std::string name, std::function<void(void)> test);
unsigned int run_tests();
std::string test_stats();

#define REGISTER_TEST(NAME) do {\
    void test_##NAME();\
    register_test_function(#NAME, test_##NAME);\
} while(0);

void success();
void fail();
void register_case();

#define TEST_CASE(X)\
    std::cout << "\n    " << #X;\
    register_case();\
    X();\

#define assert(STATEMENT) \
if (!(STATEMENT)) {\
    std::cerr << "ASSERT FAILED "\
          __FILE__ << " @" << __LINE__ << \
        ": " << #STATEMENT << std::endl;\
    fail();\
} else success();

#define assertEquals(TEST, RESULT) \
if ((TEST) == (RESULT)) {\
    success();\
}else {\
    std::cerr << "ASSERT FAILED "\
          __FILE__ << " @" << __LINE__ << \
        ": " << TEST << " != " << RESULT << std::endl;\
    fail();\
}

#define assertNotEquals(TEST, RESULT) \
if ((TEST) != (RESULT)) {\
    success();\
}else {\
    std::cerr << "ASSERT FAILED "\
          __FILE__ << " @" << __LINE__ << \
        ": " << TEST << " == " << RESULT << std::endl;\
    fail();\
}

#define assertThrows(EXPTYPE, EXPVAL, STATEMENT) \
try {\
    (STATEMENT);\
    std::cerr << std::endl\
        << "ASSERT EXCEPTION FAILED, NO EXCEPTION "\
        << __FILE__ << " @" << __LINE__\
        << ": " << #STATEMENT << std::endl;\
    fail();\
}\
catch (EXPTYPE e)\
{\
    if (e == EXPVAL) {\
        success();\
    } else {\
        std::cerr << std::endl << "ASSERT EXCEPTION FAILED "\
        << __FILE__ << " @" << __LINE__\
        << ": " << #STATEMENT << std::endl;\
        std::cerr << "Got wrong exception: " << e << std::endl;\
        fail();\
    }\
}

class StdoutCatcher
{
public:
    StdoutCatcher() : m_buf(nullptr) {}
    ~StdoutCatcher() {
        stop();
    }

    void start(bool reset=false);
    void stop();
    void reset();

    std::string get() const;

private:
    std::streambuf *m_buf;
    std::ostringstream m_stream;
};
