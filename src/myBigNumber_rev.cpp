#include "myBigNumber_rev.h"

using namespace std;

MyBigNumberRev::MyBigNumberRev(bool enableLogging, ostream& logStream)
    : enableLogging(enableLogging), logLines(&logStream) {}

ostream& MyBigNumberRev::defaultLogStream() {
    static ostream& defaultStream = cout;
    return defaultStream;
}

string MyBigNumberRev::sum(const string& stn1, const string& stn2) {
    string result;
    result.reserve(max(stn1.length(), stn2.length()) + 1);
    int i = stn1.length() - 1;
    int j = stn2.length() - 1;
    int carry = 0, digit1 = 0, digit2 = 0, carryIn = 0, step = 1;

    while (i >= 0 || j >= 0 || carry > 0) {
        digit1 = (i >= 0) ? stn1[i] - '0' : 0;
        digit2 = (j >= 0) ? stn2[j] - '0' : 0;
        carryIn = carry;
        int total = digit1 + digit2 + carryIn;
        int resultDigit = total % 10;
        carry = total / 10;

        result.push_back(resultDigit + '0');

        if (enableLogging) {
            Step(step++, digit1, digit2, carryIn, total, resultDigit, carry, result);
        }

        i--; j--;
    }

    reverse(result.begin(), result.end());
    return result;
}

void MyBigNumberRev::Step(int step, int digit1, int digit2, int previousCarry, int total, int resultDigit, int nextCarry, const std::string& partialResult) const {
    std::string cumulative = partialResult;
    std::reverse(cumulative.begin(), cumulative.end());
    *logLines << "Step " << step << ": " << digit1 << " + " << digit2 << " + " << previousCarry << " = " << total << " (Result: " << resultDigit << ", Carry: " << nextCarry << ", Cumulative Result: " << cumulative << ")" << std::endl;
}