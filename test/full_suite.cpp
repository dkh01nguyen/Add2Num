#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <chrono>
#include "myBigNumber_old.h"
#include "myBigNumber.h"
#include "myBigNumber_rev.h"

using namespace std;
using namespace std::chrono;

// ============================================================================
// 1. UNIT TESTS & STATEMENT COVERAGE
// ============================================================================
void run_unit_and_coverage_tests() {
    cout << "====================================================\n";
    cout << "   RUNNING UNIT TESTS & STATEMENT COVERAGE TESTS    \n";
    cout << "====================================================\n";

    // --- UNIT TEST CHỨC NĂNG CƠ BẢN ---
    {
        MyBigNumber mbn_new(false);
        MyBigNumberRev mbn_rev(false);
        MyBigNumberOld mbn_old(false);

        // Test không nhớ
        assert(mbn_new.sum("123", "456") == "579");
        assert(mbn_rev.sum("123", "456") == "579");
        assert(mbn_old.sum("123", "456") == "579");

        // Test có nhớ liên tiếp làm tăng chữ số kết quả
        assert(mbn_new.sum("1234", "897") == "2131");
        assert(mbn_rev.sum("1234", "897") == "2131");
        assert(mbn_old.sum("1234", "897") == "2131");

        assert(mbn_new.sum("888", "222") == "1110");
        assert(mbn_rev.sum("888", "222") == "1110");
        assert(mbn_old.sum("888", "222") == "1110");

        assert(mbn_new.sum("12345678901234567890", "98765432109876543210") == "111111111011111111100");
        assert(mbn_rev.sum("12345678901234567890", "98765432109876543210") == "111111111011111111100");
        assert(mbn_old.sum("12345678901234567890", "98765432109876543210") == "111111111011111111100");

        cout << " -> [PASS] Basic Functionality Tests\n";
    }

    // --- COVERAGE TEST CHO PHIÊN BẢN CŨ ---
    {
        stringstream log_stream;
        stringstream log_stream_false;

        MyBigNumberOld mbn_old_new_dummy;

        // Bật enableLogging = true để bao phủ dòng lệnh chứa hàm Step() của bản cũ
        MyBigNumberOld mbn_old(true, log_stream);

        // Nhánh độ dài lệch nhau để bao phủ lệnh đệm chuỗi '0'
        // Nhánh carry > 0 cuối cùng để bao phủ lệnh xử lý kết quả sau vòng lặp
        string r1 = mbn_old.sum("9", "9"); 
        assert(r1 == "18");
        
        string r2 = mbn_old.sum("1", "123");
        assert(r2 == "124");

        // Đảm bảo luồng log hoạt động để chứng minh hàm Step đã được bao phủ
        assert(!log_stream.str().empty());
        MyBigNumberOld mbn_oldd(false, log_stream_false);
        assert(log_stream_false.str().empty()); // Đảm bảo log không bị ảnh hưởng bởi instance mới với enableLogging = false
        cout << " -> [PASS] Statement Coverage for the old version\n";
    }

    // --- COVERAGE TEST CHO PHIÊN BẢN REV ---
    {
        stringstream log_stream;
        stringstream log_stream_false;

        MyBigNumberRev mbn_rev(true, log_stream);

        string r1 = mbn_rev.sum("12", "987");
        assert(r1 == "999");

        string r2 = mbn_rev.sum("99", "99");
        assert(r2 == "198");

        assert(!log_stream.str().empty());

        MyBigNumberRev mbn_revv(false, log_stream_false);
        assert(log_stream_false.str().empty());
        cout << " -> [PASS] Statement Coverage for the rev version\n";
    }

    // --- COVERAGE TEST CHO PHIÊN BẢN MỚI ---
    {
        stringstream log_stream;
        stringstream log_stream_false;

        MyBigNumber mbn_new_dummy;    // khởi tạo mặc định, flag = true, để bao phủ hàm defaultLogStream(), sẽ báo lỗi hàm defaultLogStream() nếu có sai sót ở đây

        // Bật enableLogging = true để bao phủ dòng lệnh chứa hàm Step() của bản mới
        MyBigNumber mbn_new(true, log_stream);

        // Trường hợp stn2 dài hơn stn1 -> Bao phủ nhánh hoán đổi con trỏ (longer / shorter)
        string r1 = mbn_new.sum("12", "987");
        assert(r1 == "999");

        // Trường hợp cộng tràn hàng cao nhất vượt quá chiều dài chuỗi gốc
        // -> Vòng lặp chạy tiếp khi i < 0 nhưng carry > 0 -> Bao phủ nhánh 'else' của (i >= 0) chứa lệnh result.insert
        string r2 = mbn_new.sum("99", "99");
        assert(r2 == "198");

        assert(!log_stream.str().empty());

        MyBigNumber mbn_neww(false, log_stream_false);
        assert(log_stream_false.str().empty()); // Đảm bảo log không bị ảnh hưởng bởi instance mới với enableLogging = false
        cout << " -> [PASS] Statement Coverage for the new version\n";
    }
}

// ============================================================================
// 2. MEMORY & PERFORMANCE BENCHMARK (STRESS TEST)
// ============================================================================
// void run_memory_and_performance_benchmark() {
//     cout << "\n=======================================================\n";
//     cout << "  2. RUNNING MEMORY & PERFORMANCE STRESS TEST          \n";
//     cout << "=======================================================\n";
    
//     const int num_digits = 100000; // Số chữ số khổng lồ để thấy sự chênh lệch bộ nhớ/CPU
//     cout << "2 big numbers with " << num_digits << " digits '9'...\n";

//     string big_num1(num_digits, '9');
//     string big_num2(num_digits, '9');

//     // Khảo sát phiên bản mới (Tối ưu hóa ghi đè)
//     long long duration_new = 0;
//     {
//         MyBigNumber mbn_new(false); // Tắt log để tránh tốn RAM hệ thống cho việc in ấn dữ liệu
//         cout << " -> New version running... " << flush;
        
//         auto start = high_resolution_clock::now();
//         string res_new = mbn_new.sum(big_num1, big_num2);
//         auto end = high_resolution_clock::now();
        
//         duration_new = duration_cast<milliseconds>(end - start).count();
//         cout << "Completed in: " << duration_new << " ms.\n";
//     }

//     // Khảo sát phiên bản cũ (Liên tục chèn insert và padding tạo chuỗi tạm)
//     long long duration_old = 0;
//     {
//         MyBigNumberOld mbn_old(false);
//         cout << " -> Old version running... " << flush;
        
//         auto start = high_resolution_clock::now();
//         string res_old = mbn_old.sum(big_num1, big_num2);
//         auto end = high_resolution_clock::now();
        
//         duration_old = duration_cast<milliseconds>(end - start).count();
//         cout << "Completed in: " << duration_old << " ms.\n";
//     }
// }

int main() {
    run_unit_and_coverage_tests();
    // run_memory_and_performance_benchmark();
    return 0;
}