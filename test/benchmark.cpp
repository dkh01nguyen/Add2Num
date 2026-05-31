#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <cassert>

// Platform-specific headers for accurate memory tracking via OS API
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#include <unistd.h>
#endif

// Function to get the current Memory Usage (Resident Set Size / Working Set) in Bytes
size_t get_current_memory_usage() {
#if defined(_WIN32) || defined(_WIN64)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
#else
    // Long format parsing for Linux /proc/self/statm (Page size * Resident Pages)
    long pages = 0;
    size_t page_size = sysconf(_SC_PAGESIZE);
    FILE* fp = fopen("/proc/self/statm", "r");
    if (fp) {
        if (fscanf(fp, "%*s %ld", &pages) == 1) {
            fclose(fp);
            return pages * page_size;
        }
        fclose(fp);
    }
    // Fallback for macOS/BSD using getrusage
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return usage.ru_maxrss * 1024; // Convert KB to Bytes
    }
    return 0;
#endif
}

// ============================================================================
// PROJECT IMPLEMENTATIONS (INLINED)
// ============================================================================

class MyBigNumberOldImpl {
public:
    MyBigNumberOldImpl(bool enableLogging = false, std::ostream& logStream = std::cout)
        : enableLogging(enableLogging), logLines(&logStream) {}

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

            result.insert(result.begin(), resultDigit + '0');
        }

        if (carry > 0) {
            result.insert(result.begin(), carry + '0');
        }

        return result;
    }
private:
    bool enableLogging;
    std::ostream* logLines;
};

class MyBigNumberNewImpl {
public:
    MyBigNumberNewImpl(bool enableLogging = false, std::ostream& logStream = std::cout)
        : enableLogging(enableLogging), logLines(&logStream) {}

    std::string sum(const std::string& stn1, const std::string& stn2) {
        const std::string* longer = &stn1;
        const std::string* shorter = &stn2;
        
        if (stn2.length() > stn1.length()) {
            longer = &stn2;
            shorter = &stn1;
        }

        std::string result = *longer;
        int i = static_cast<int>(longer->length()) - 1;
        int j = static_cast<int>(shorter->length()) - 1;
        int carry = 0, digit1 = 0, digit2 = 0, carryIn = 0;
        int total = 0, resultDigit = 0;

        while (j >= 0 || carry > 0) {
            digit1 = (i >= 0) ? result[i] - '0' : 0;
            digit2 = (j >= 0) ? (*shorter)[j] - '0' : 0;
            carryIn = carry;
            total = digit1 + digit2 + carryIn;
            resultDigit = total % 10;
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
private:
    bool enableLogging;
    std::ostream* logLines;
};

static std::string sum_old(const std::string& a, const std::string& b) {
    MyBigNumberOldImpl impl;
    return impl.sum(a, b);
}

static std::string sum_new(const std::string& a, const std::string& b) {
    MyBigNumberNewImpl impl;
    return impl.sum(a, b);
}

// // ============================================================================
// // AUTOMATED TESTS
// // ============================================================================
// void run_automated_tests() {
//     std::cout << "==================================================\n";
//     std::cout << "RUNNING UNIT TESTS & STATEMENT COVERAGE TESTS\n";
//     std::cout << "==================================================\n";

//     assert(sum_old("123", "456") == "579");
//     assert(sum_new("123", "456") == "579");
//     assert(sum_old("99", "1") == "100");
//     assert(sum_new("99", "1") == "100");
    
//     std::cout << " -> [PASS] Functional logic assertions verified.\n\n";
// }

// ============================================================================
// BENCHMARK RUNNER (SYSTEM-LEVEL MEMORY TRACKING)
// ============================================================================
void run_benchmark(const std::string& label, const std::string& n1, const std::string& n2, int iterations) {
    std::cout << "==================================================\n";
    std::cout << "BENCHMARK: " << label << "\n";
    std::cout << "String lengths: " << n1.length() << " and " << n2.length() << " digits\n";
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "==================================================\n";

    std::string ref_result;

    // --- MEASURE OLD VERSION ---
    uint64_t total_time_old = 0;
    size_t peak_mem_old = 0;

    for (int it = 0; it < iterations; ++it) {
        size_t mem_before = get_current_memory_usage();
        
        auto start = std::chrono::high_resolution_clock::now();
        std::string res = sum_old(n1, n2);
        auto end = std::chrono::high_resolution_clock::now();
        
        size_t mem_after = get_current_memory_usage();

        if (it == 0) ref_result = res;

        auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        total_time_old += (uint64_t)dur;
        
        if (mem_after > mem_before) {
            size_t diff = mem_after - mem_before;
            if (diff > peak_mem_old) peak_mem_old = diff;
        }
    }

    // --- MEASURE NEW VERSION ---
    uint64_t total_time_new = 0;
    size_t peak_mem_new = 0;

    for (int it = 0; it < iterations; ++it) {
        size_t mem_before = get_current_memory_usage();
        
        auto start = std::chrono::high_resolution_clock::now();
        std::string res = sum_new(n1, n2);
        auto end = std::chrono::high_resolution_clock::now();
        
        size_t mem_after = get_current_memory_usage();

        if (res != ref_result) {
            std::cerr << "ERROR: Results do not match!\n";
            return;
        }

        auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        total_time_new += (uint64_t)dur;
        
        if (mem_after > mem_before) {
            size_t diff = mem_after - mem_before;
            if (diff > peak_mem_new) peak_mem_new = diff;
        }
    }

    // --- PRINT RESULTS TABLE ---
    double avg_time_old = (double)total_time_old / iterations;
    double avg_time_new = (double)total_time_new / iterations;

    const int metric_width = 25;
    const int value_width = 25;

    auto print_row = [&](const std::string& metric, const std::string& old_value, const std::string& new_value) {
        std::cout << std::left << std::setw(metric_width) << metric
                  << " | " << std::right << std::setw(value_width) << old_value
                  << " | " << std::setw(value_width) << new_value << "\n";
    };

    auto make_separator = [&]() {
        return std::string(metric_width, '-') + "-+-" + std::string(value_width, '-') + "-+-" + std::string(value_width, '-');
    };

    std::ostringstream old_time, new_time;
    old_time << std::fixed << std::setprecision(4) << avg_time_old;
    new_time << std::fixed << std::setprecision(4) << avg_time_new;

    std::ostringstream old_mem_str, new_mem_str;
    // Format output to KB for better readability
    old_mem_str << std::fixed << std::setprecision(2) << (double)peak_mem_old << " B";
    new_mem_str << std::fixed << std::setprecision(2) << (double)peak_mem_new << " B";

    print_row("Metric", "Old Version", "New Version");
    std::cout << make_separator() << "\n";
    print_row("Avg Time (us)", old_time.str(), new_time.str());
    print_row("Peak Delta Memory", old_mem_str.str(), new_mem_str.str());
    std::cout << "\n";
}

int main() {
    // run_automated_tests();

    // Small dataset
    run_benchmark("Small Numbers", "12345", "67890", 10000);

    // Ultra Large dataset to force real heap impact
    // We increase size to 50,000 digits to bypass OS lazy allocation caching boundaries
    std::string big_a(50000, '9');
    big_a[0] = '1';
    std::string big_b(50000, '9');
    run_benchmark("Large 50k-Digit Numbers (Stress Test)", big_a, big_b, 1000);

    return 0;
}