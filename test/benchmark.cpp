#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

// Global counters for memory tracking
size_t global_allocations = 0;
size_t global_bytes_allocated = 0;

// Overriding global new to intercept allocations
void* operator new(size_t size) {
    global_allocations++;
    global_bytes_allocated += size;
    return malloc(size);
}

void operator delete(void* p) noexcept {
    free(p);
}

void reset_memory_counters() {
    global_allocations = 0;
    global_bytes_allocated = 0;
}

// Inline the project's two implementations here so the benchmark is self-contained.
// This avoids cross-file class-name collisions and keeps the benchmark single-file.

// --- Old implementation (copied from src/myBigNumber_old.cpp) ---
class MyBigNumberOldImpl {
public:
    std::string sum(std::string stn1, std::string stn2) {
        std::string result;
        int carry = 0;
        int maxLength = std::max(stn1.length(), stn2.length());

        stn1 = std::string(maxLength - stn1.length(), '0') + stn1;
        stn2 = std::string(maxLength - stn2.length(), '0') + stn2;

        for (int i = maxLength - 1; i >= 0; --i) {
            int digit1 = stn1[i] - '0';
            int digit2 = stn2[i] - '0';
            int carryIn = carry;
            int total = digit1 + digit2 + carryIn;
            int resultDigit = total % 10;
            carry = total / 10;

            // O(N) shift insertion
            result.insert(result.begin(), resultDigit + '0');
        }

        if (carry > 0) {
            result.insert(result.begin(), carry + '0');
        }

        return result;
    }
};

// --- New implementation (copied from src/myBigNumber.cpp) ---
class MyBigNumberNewImpl {
public:
    std::string sum(const std::string& stn1, const std::string& stn2) {
        std::string result;
        result.reserve(std::max(stn1.length(), stn2.length()) + 1);

        int carry = 0;
        int i = (int)stn1.length() - 1;
        int j = (int)stn2.length() - 1;

        while (i >= 0 || j >= 0 || carry > 0) {
            int digit1 = (i >= 0) ? (stn1[i] - '0') : 0;
            int digit2 = (j >= 0) ? (stn2[j] - '0') : 0;
            int total = digit1 + digit2 + carry;
            carry = total / 10;

            // O(1) back insertion
            result.push_back((total % 10) + '0');

            i--;
            j--;
        }

        std::reverse(result.begin(), result.end());
        return result;
    }
};

// Small adapters to keep the benchmark code below unchanged.
static std::string sum_old(const std::string& a, const std::string& b) {
    MyBigNumberOldImpl impl;
    return impl.sum(a, b);
}

static std::string sum_new(const std::string& a, const std::string& b) {
    MyBigNumberNewImpl impl;
    return impl.sum(a, b);
}

// ==========================================
// BENCHMARK RUNNER
// ==========================================
void run_benchmark(const std::string& label, const std::string& n1, const std::string& n2, int iterations) {
    std::cout << "\n==================================================\n";
    std::cout << "BENCHMARK: " << label << "\n";
    std::cout << "String lengths: " << n1.length() << " and " << n2.length() << " digits\n";
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "==================================================\n";

    // Warm-up single run to ensure lazy allocations are done
    reset_memory_counters();
    volatile std::string warm_old = sum_old(n1, n2);
    reset_memory_counters();
    volatile std::string warm_new = sum_new(n1, n2);

    // Old version measurements
    uint64_t total_time_old = 0;
    uint64_t total_allocs_old = 0;
    uint64_t total_bytes_old = 0;
    std::string ref_result;

    for (int it = 0; it < iterations; ++it) {
        reset_memory_counters();
        auto start = std::chrono::high_resolution_clock::now();
        std::string res = sum_old(n1, n2);
        auto end = std::chrono::high_resolution_clock::now();

        if (it == 0) ref_result = res;

        auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        total_time_old += (uint64_t)dur;
        total_allocs_old += global_allocations;
        total_bytes_old += global_bytes_allocated;
    }

    // New version measurements
    uint64_t total_time_new = 0;
    uint64_t total_allocs_new = 0;
    uint64_t total_bytes_new = 0;

    for (int it = 0; it < iterations; ++it) {
        reset_memory_counters();
        auto start = std::chrono::high_resolution_clock::now();
        std::string res = sum_new(n1, n2);
        auto end = std::chrono::high_resolution_clock::now();

        if (res != ref_result) {
            std::cerr << "ERROR: Results do not match between implementations!\n";
            return;
        }

        auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        total_time_new += (uint64_t)dur;
        total_allocs_new += global_allocations;
        total_bytes_new += global_bytes_allocated;
    }

    double avg_time_old = (double)total_time_old / iterations;
    double avg_time_new = (double)total_time_new / iterations;
    double avg_allocs_old = (double)total_allocs_old / iterations;
    double avg_allocs_new = (double)total_allocs_new / iterations;
    double avg_bytes_old = (double)total_bytes_old / iterations;
    double avg_bytes_new = (double)total_bytes_new / iterations;

    std::cout << "Metric                  | Old Version        | New Version\n";
    std::cout << "------------------------|--------------------|--------------------\n";
    std::cout << "Avg Time (us)          | " << avg_time_old << "              | " << avg_time_new << "\n";
    std::cout << "Avg Heap Allocs         | " << avg_allocs_old << "                 | " << avg_allocs_new << "\n";
    std::cout << "Avg Bytes Allocated     | " << avg_bytes_old << " B             | " << avg_bytes_new << " B\n";

    if (avg_time_new > 0.0) {
        std::cout << "Performance Gain: " << (avg_time_old / avg_time_new) << "x faster\n";
    }
}

int main(int argc, char** argv) {
    // Small numbers (many iterations)
    run_benchmark("Small Numbers", "12345", "67890", 10000);

    // Large numbers (fewer iterations)
    std::string big_a(5000, '9');
    std::string big_b(5000, '1');
    run_benchmark("Large 5,000-Digit Numbers", big_a, big_b, 20);

    return 0;
}
