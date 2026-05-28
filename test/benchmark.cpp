#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <new>

// Global counters for memory tracking
size_t global_allocations = 0;
size_t global_bytes_allocated = 0;

// Overriding global new to intercept allocations
void* operator new(size_t size) {
    global_allocations++;
    global_bytes_allocated += size;
    void* p = malloc(size);
    if (p == nullptr) {
        throw std::bad_alloc();
    }

    return p;
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

// --- New implementation (copied from src/myBigNumber.cpp) ---
class MyBigNumberNewImpl {
public:
    std::string sum(const std::string& stn1, const std::string& stn2) {
        size_t maxLength = std::max(stn1.length(), stn2.length());

        const std::string* longer = &stn1;
        const std::string* shorter = &stn2;
        if (stn2.length() > stn1.length()) {
            longer = &stn2;
            shorter = &stn1;
        }

        std::string result = *longer;
        int i = static_cast<int>(longer->length()) - 1;
        int j = static_cast<int>(shorter->length()) - 1;
        int carry = 0, digit1 = 0, digit2 = 0, carryIn = 0, step = 1;

        while (j >= 0 || carry > 0) {
            digit1 = (i >= 0) ? result[i] - '0' : 0;
            digit2 = (j >= 0) ? (*shorter)[j] - '0' : 0;
            carryIn = carry;
            int total = digit1 + digit2 + carryIn;
            int resultDigit = total % 10;
            carry = total / 10;

            if (i >= 0) {
                result[i] = resultDigit + '0';
            } else {
                result.insert(result.begin(), resultDigit + '0');
            }
            i--; j--;
        }

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

    const int metric_width = 25;
    const int value_width = 50;

    auto print_row = [&](const std::string& metric, const std::string& old_value, const std::string& new_value) {
        std::cout << std::left << std::setw(metric_width) << metric
                  << " | " << std::right << std::setw(value_width) << old_value
                  << " | " << std::setw(value_width) << new_value << "\n";
    };

    auto make_separator = [&]() {
        return std::string(metric_width, '-') + "-+-" + std::string(value_width, '-') + "-+-" + std::string(value_width, '-');
    };

    std::ostringstream old_time;
    old_time << std::fixed << std::setprecision(4) << avg_time_old;
    std::ostringstream new_time;
    new_time << std::fixed << std::setprecision(4) << avg_time_new;

    print_row("Metric", "Old Version", "New Version");
    std::cout << make_separator() << "\n";
    print_row("Avg Time (us)", old_time.str(), new_time.str());
}

int main() {
    // Small numbers (many iterations)
    run_benchmark("Small Numbers", "12345", "67890", 10000);

    // Large numbers (fewer iterations)
    std::string big_a(1000000, '0');
    big_a[0] = '1'; // Make it 1 million digits but not all zeros
    std::string big_b(1, '1');
    run_benchmark("Large 1 million-Digit Numbers", big_a, big_b, 1000000);

    return 0;
}
