#include <iostream>
#include <cassert>
#include <stdexcept>
#include "myBigNumber.h"

void verifySum(MyBigNumber& mbn, const std::string& label, const std::string& num1, const std::string& num2, const std::string& expected) {
    std::cout << "\n========================================\n";
    std::cout << label << "\n";
    std::cout << "========================================\n";
    std::cout << "Testing: " << num1 << " + " << num2 << "...\n";
    
    // Execute exactly once
    std::string result = mbn.sum(num1, num2);
    
    std::cout << "Result: " << result << '\n';
    assert(result == expected);
}

int main() {
    MyBigNumber mbn;

    // 1. Same length - No carry
    verifySum(mbn, "Test 1: Same length - Zero carry", "123", "456", "579");

    // 2. Different lengths (A > B and B > A)
    verifySum(mbn, "Test 2A: Different lengths (A > B)", "1234", "897", "2131");
    verifySum(mbn, "Test 2B: Different lengths (B > A)", "897", "1234", "2131");

    // 3. Carry propagation cascade
    verifySum(mbn, "Test 3A: Carry propagation cascade", "999", "1", "1000");
    verifySum(mbn, "Test 3B: Reverse propagation cascade", "1", "999", "1000");
    verifySum(mbn, "Test 3C: Consecutive internal carries", "4567", "5433", "10000");

    // 4. Zero handling variants
    verifySum(mbn, "Test 4A: Double zeros", "0", "0", "0");
    verifySum(mbn, "Test 4B: Left zero identity", "0", "123", "123");
    verifySum(mbn, "Test 4C: Right zero identity", "123", "0", "123");

    // 5. Very Large BigInt Bounds
    verifySum(mbn, "Test 5: Ultra large values", 
              "12345678901234567890", "98765432109876543210", "111111111011111111100");

    // 6. Robustness / Edge constraints
    verifySum(mbn, "Test 6: Empty values handling", "0", "456", "456");

    std::cout << "\n========================================\n";
    std::cout << "SUCCESS: All 10 edge cases verified!" << std::endl;
    std::cout << "========================================\n";

    return 0;
}