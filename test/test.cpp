#include <iostream>
#include <cassert>
#include "../inc/myBigNumber.h"

void test1() {
    MyBigNumber mbn;
    std::cout << "\n========================================\n";
    std::cout << "Test 1: Same length - Zero carry" << std::endl;
    std::cout << "========================================\n";
    // Same length without carry
    std::cout << "Testing sum of 123 and 456..." << std::endl;
    assert(mbn.sum("123", "456") == "579");
    std::string result = mbn.sum("123", "456");

    std::cout << "Result: " << result << '\n';
}

void test2() {
    MyBigNumber mbn;
    std::string result;
    std::cout << "\n========================================\n";
    std::cout << "Test 2: Different lengths" << std::endl;
    std::cout << "========================================\n";

    std::cout << "Testing sum of 1234 and 897..." << std::endl;
    assert(mbn.sum("1234", "897") == "2131");
    result = mbn.sum("1234", "897");
    std::cout << "Result: " << result << '\n';

    std::cout << "\n========================================\n";
    std::cout << "Testing sum of 1 and 99..." << std::endl;
    assert(mbn.sum("1", "99") == "100");
    result = mbn.sum("1", "99");
    std::cout << "Result: " << result << '\n';
}

void test3() {
    MyBigNumber mbn;
    std::string result;
    std::cout << "\n========================================\n";
    std::cout << "Test 3: Carry propagation" << std::endl;
    std::cout << "========================================\n";
    std::cout << "Testing sum of 999 and 1..." << std::endl;
    assert(mbn.sum("999", "1") == "1000");
    result = mbn.sum("999", "1");
    std::cout << "Result: " << result << '\n';

    std::cout << "\n========================================\n";
    std::cout << "Testing sum of 1 and 999..." << std::endl;
    assert(mbn.sum("1", "999") == "1000");
    result = mbn.sum("1", "999");
    std::cout << "Result: " << result << '\n';
}

void test4() {
    MyBigNumber mbn;
    std::string result;
    std::cout << "\n========================================\n";
    std::cout << "Test 4: Zeroes handling" << std::endl;
    std::cout << "========================================\n";
    // Zeroes handling
    std::cout << "Testing sum of 0 and 0..." << std::endl;
    assert(mbn.sum("0", "0") == "0");
    result = mbn.sum("0", "0");
    std::cout << "Result: " << result << '\n';

    std::cout << "\n========================================\n";
    std::cout << "Testing sum of 0 and 123..." << std::endl;
    assert(mbn.sum("0", "123") == "123");
    result = mbn.sum("0", "123");
    std::cout << "Result: " << result << '\n';

    std::cout << "\n========================================\n";
    std::cout << "And reverse..." << std::endl;
    assert(mbn.sum("123", "0") == "123");
    result = mbn.sum("123", "0");
    std::cout << "Result: " << result << '\n';
}

void test5() {
    MyBigNumber mbn;
    std::string result;
    std::cout << "\n========================================\n";
    std::cout << "Test 5: Very large numbers" << std::endl;
    std::cout << "========================================\n";
    std::cout << "Testing sum of 12345678901234567890 and 98765432109876543210..." << std::endl;
    assert(mbn.sum("12345678901234567890", "98765432109876543210") == "111111111011111111100");
    result = mbn.sum("12345678901234567890", "98765432109876543210");
    std::cout << "Result: " << result << '\n';
}

void runTests() {
    test1();
    test2();
    test3();
    test4();
    test5();
    std::cout << "\n========================================\n";
    std::cout << "SUCCESS: All test cases passed!" << std::endl;
    std::cout << "========================================\n";
}

int main() {
    runTests();
    return 0;
}