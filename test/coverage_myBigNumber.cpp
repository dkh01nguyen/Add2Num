#include <cassert>
#include <sstream>
#include <string>

#include "myBigNumber.h"

static void test_basic_no_carry() {
    std::ostringstream log;
    MyBigNumber mbn(true, log);
    const std::string result = mbn.sum("123", "456");
    assert(result == "579");
    assert(!log.str().empty());
}

static void test_length_swap_branch() {
    std::ostringstream log;
    MyBigNumber mbn(true, log);
    const std::string result = mbn.sum("12", "987");
    assert(result == "999");
    assert(!log.str().empty());
}

static void test_carry_inserts_new_leading_digit() {
    std::ostringstream log;
    MyBigNumber mbn(true, log);
    const std::string result = mbn.sum("99", "99");
    assert(result == "198");
    assert(!log.str().empty());
}

static void test_logging_disabled() {
    std::ostringstream log;
    MyBigNumber mbn(false, log);
    const std::string result = mbn.sum("500", "500");
    assert(result == "1000");
    assert(log.str().empty());
}

static void test_default_constructor_stream_path() {
    // This covers the default constructor path that uses defaultLogStream().
    MyBigNumber mbn;
    const std::string result = mbn.sum("0", "0");
    assert(result == "0");
}

int main() {
    test_basic_no_carry();
    test_length_swap_branch();
    test_carry_inserts_new_leading_digit();
    test_logging_disabled();
    test_default_constructor_stream_path();
    return 0;
}
