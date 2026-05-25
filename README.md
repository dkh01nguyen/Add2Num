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
|-- README.md
```

File meanings:

```text
inc/myBigNumber.h         Declares the MyBigNumber class
src/myBigNumber.cpp       Implements the big-number addition algorithm
src/main.cpp              Demo program
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
g++ src/myBigNumber.cpp src/main.cpp -o add2num.exe
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

## Notes

- The current program handles non-negative integers only.
- If you want a more complete layout like the sample, you can add a `tests/` folder and a separate test file later.
- The log output is printed to help follow each addition step and carry value.