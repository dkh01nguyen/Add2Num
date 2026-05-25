#ifndef MY_BIG_NUMBER_H
#define MY_BIG_NUMBER_H

#include <iostream>
#include <algorithm>
#include <string>

class MyBigNumber {
public:
    MyBigNumber(bool enableLogging = true, std::ostream& logStream = defaultLogStream());
    std::string sum(std::string stn1, std::string stn2);

private:
    bool enableLogging;
    std::ostream* logLines;

    static std::ostream& defaultLogStream();

    void Step(int step, int digit1, int digit2, int previousCarry, int total, int resultDigit, int nextCarry, const std::string& partialResult) const;
};

#endif // MY_BIG_NUMBER_H