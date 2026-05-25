#include "myBigNumber.h"

using namespace std;

MyBigNumber::MyBigNumber(bool enableLogging, ostream& logStream)
    : enableLogging(enableLogging), logLines(&logStream) {}

ostream& MyBigNumber::defaultLogStream() {
    static ostream& defaultStream = cout;
    return defaultStream;
}

string MyBigNumber::sum(string stn1, string stn2) {
    string result;
    int carry = 0;
    int maxLength = max(stn1.length(), stn2.length());

    stn1 = string(maxLength - stn1.length(), '0') + stn1;
    stn2 = string(maxLength - stn2.length(), '0') + stn2;

    for (int i = maxLength - 1; i >= 0; --i) {
        int digit1 = stn1[i] - '0';
        int digit2 = stn2[i] - '0';
        int carryIn = carry;
        int total = digit1 + digit2 + carryIn;
        int resultDigit = total % 10;
        carry = total / 10;

        result.insert(result.begin(), resultDigit + '0');

        if (enableLogging) {
            Step(maxLength - i, digit1, digit2, carryIn, total, resultDigit, carry, result);
        }
    }

    if (carry > 0) {
        result.insert(result.begin(), carry + '0');
    }

    return result;
}

void MyBigNumber::Step(int step, int digit1, int digit2, int previousCarry,int total, int resultDigit, int nextCarry, const std::string& partialResult) const {
    *logLines << "Step " << step << ": " << digit1 << " + " << digit2 << " + " << previousCarry << " = " << total << " (Result: " << resultDigit << ", Carry: " << nextCarry << ")" << std::endl;
}