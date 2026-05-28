# Add2Num

Add2Num is a C++ project that implements addition for two large numbers represented as strings. It adds digits from right to left, just like elementary-school addition.

Example: "1234" + "897" = "2131"

## Project Structure

```text
Add2Num/
|-- inc/
|   |-- myBigNumber.h
|-- src/
|   |-- myBigNumber.cpp
|   |-- main.cpp
|-- test/
|   |-- demo.cpp
|-- README.md
```

File meanings:

```text
inc/myBigNumber.h         Declares the MyBigNumber class
src/myBigNumber.cpp       Implements the big-number addition algorithm
src/main.cpp              Demo program
test/demo.cpp             Testcases
README.md                 Build and usage guide
```

## Main Method

The `MyBigNumber` class provides the following method:

```cpp
string sum(string stn1, string stn2);
```

Where:

```text
stn1    First number, represented as a string
stn2    Second number, represented as a string
return  The sum of stn1 and stn2, represented as a string
```

According to the problem statement, the input is assumed to be valid and contains only digits from `0` to `9`, so the program does not perform input validation.

## Algorithm

The algorithm works as follows:

```text
1. Traverse stn1 and stn2 from right to left at the same time.
2. Read each character and convert it to a digit.
3. Add the two digits together with the carry value.
4. Store the ones digit in the temporary result.
5. Keep the carry for the next step.
6. After the traversal, append the remaining carry if it exists.
7. Reverse the temporary result to obtain the final answer.
```

The program prints the history of each addition step.

## Environment Requirements

A C++ compiler that supports C++17 is required, for example `g++`.

Check `g++`:

```powershell
g++ --version
```

## Build

Clone the repository from GitHub and change into the project directory first:

```powershell
git clone https://github.com/dkh01nguyen/Add2Num.git
cd Add2Num
```

Then run the following command from the `Add2Num` directory:

```powershell
g++ -Iinc src/myBigNumber.cpp src/main.cpp -o add2num.exe
```

Or a stricter version:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinc src/myBigNumber.cpp src/main.cpp -o add2num.exe
```

## Run

The program accepts 2 arguments. To run it, use:

```powershell
./add2num.exe 1234 897
```

Expected output:

```text
Step 1: 4 + 7 + 0 = 11 (Result: 1, Carry: 1)
Step 2: 3 + 9 + 1 = 13 (Result: 3, Carry: 1)
Step 3: 2 + 8 + 1 = 11 (Result: 1, Carry: 1)
Step 4: 1 + 0 + 1 = 2 (Result: 2, Carry: 0)
Result: 2131
```

If you run it without arguments:

```powershell
./add2num.exe
```

The program will print:

```text
Please provide two numbers as command-line arguments.
```

## Tests

A separate test suite is provided to verify the core addition logic against different edge cases (e.g., carry propagation, different string lengths, and zero values). The tests use standard C++ assertions.

A separate test suite (`test/demo.cpp`) is provided to verify the core addition logic against various edge cases, such as carry propagation, different string lengths, and zero values. This project uses the standard C++ `<cassert>` library for testing rather than a heavy external framework.

### How `assert` Works

The `assert(expression)` macro evaluates a given condition:

* **If the condition is `true`** (e.g., `mbn.sum("123", "456") == "579"`), the program quietly continues to the next line.

* **If the condition is `false`**, the program immediately aborts execution and prints an error message to the terminal showing exactly which file and line number failed. This makes debugging very straightforward.

**To build the tests:**
Run the following command from the root of the `Add2Num` directory. This compiles the big-number logic along with the test file instead of the main application.

```powershell
g++ -Iinc src/myBigNumber.cpp test/demo.cpp -o run_tests.exe
./run_tests.exe
```

Or a stricter version:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinc src/myBigNumber.cpp test/demo.cpp -o run_tests.exe
./run_tests.exe
```
## Benchmark

An improved micro-benchmark testbench has been added at [test/benchmark.cpp](test/benchmark.cpp). It compares the original (0.0.1 version) O(N^2)-style implementation (`src/myBigNumber_old.cpp`) against the optimized (0.0.2) O(N) implementation (`src/myBigNumber.cpp`) across multiple iterations, measuring average execution time and average heap allocations/bytes. The benchmark is self-contained — both implementations are inlined in `test/benchmark.cpp` so a single compile step is sufficient.

To compile and run the benchmark:

```powershell
g++ -std=c++17 -O2 test/benchmark.cpp -o benchmark.exe
.\benchmark.exe
```

Notes:

* The testbench runs a high-iteration small-number test and a lower-iteration large-number test to highlight time and allocation differences.
* Results show average microsecond timings and average heap allocation counts/bytes for each implementation.
* You can adjust iteration counts by editing `test/benchmark.cpp` and recompiling.

### Sample benchmark results

The following is a short sample output from running the testbench on a development machine:

```text
BENCHMARK: Small Numbers (5 and 5 digits, 10000 iterations)
Avg Time (us) - Old: 0.0008    New: 0
Avg Heap Allocs - Old: 0       New: 0
Avg Bytes Allocated - Old: 0 B New: 0 B

BENCHMARK: Large 5,000-Digit Numbers (20 iterations)
Avg Time (us) - Old: 124.5     New: 9
Avg Heap Allocs - Old: 0       New: 0
Avg Bytes Allocated - Old: 0 B New: 0 B

Performance gain: ~13.8x faster (Old / New)
```

## Notes

* The current program handles non-negative integers only.
* The log output is printed to help follow each addition step and carry value.
