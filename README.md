# Add2Num

Add2Num is a small C++ command-line program that adds two large non-negative integers represented as strings. It is useful when the numbers are too large to fit in built-in integer types.

The project also prints step-by-step logging for each digit addition, including carry handling, so you can follow the algorithm as it runs.

## Features

- Adds arbitrarily large whole numbers using string-based arithmetic
- Preserves leading-digit carry when the sum grows in length
- Prints a readable step-by-step trace of each addition step

## Project Structure

- `inc/myBigNumber.h` - class declaration
- `src/myBigNumber.cpp` - big-number addition logic and logging
- `src/main.cpp` - command-line interface

## Build

From the project root, compile with `g++`:

```bash
g++ src/myBigNumber.cpp src/main.cpp -o main.exe
```

If you prefer a warning-heavy build, you can also use:

```bash
g++ -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -g3 -O0 src/myBigNumber.cpp src/main.cpp -o main.exe
```

## Usage

Run the program with two numbers as command-line arguments:

```powershell
./main.exe 1234 5567
```

Example output:

```text
Step 1: 4 + 7 + 0 = 11 (Result: 1, Carry: 1)
Step 2: 3 + 6 + 1 = 10 (Result: 0, Carry: 1)
Step 3: 2 + 5 + 1 = 8 (Result: 8, Carry: 0)
Step 4: 1 + 5 + 0 = 6 (Result: 6, Carry: 0)
Result: 6801
```

## Notes

- The program expects exactly two arguments.
- Input should contain digits only.
- The current implementation is intended for non-negative integers.