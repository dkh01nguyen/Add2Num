#include "myBigNumber.h"

using namespace std;

MyBigNumber::MyBigNumber(bool enableLogging, ostream& logStream)
    : enableLogging(enableLogging), logLines(&logStream) {}

ostream& MyBigNumber::defaultLogStream() {
    static ostream& defaultStream = cout;
    return defaultStream;
}

string MyBigNumber::sum(const string& stn1, const string& stn2) {
    const string* longer = &stn1;
    const string* shorter = &stn2;
    if (stn2.length() > stn1.length()) {
        longer = &stn2;
        shorter = &stn1;
    }

    string result = *longer;
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

        if (enableLogging) {
            string partialResult = result.substr(result.length() - step);
            reverse(partialResult.begin(), partialResult.end());
            Step(step++, digit1, digit2, carryIn, total, resultDigit, carry, partialResult);
        }

        i--; j--;
    }

    return result;
}

void MyBigNumber::Step(int step, int digit1, int digit2, int previousCarry, int total, int resultDigit, int nextCarry, const std::string& partialResult) const {
    std::string cumulative = partialResult;
    std::reverse(cumulative.begin(), cumulative.end());
    *logLines << "Step " << step << ": " << digit1 << " + " << digit2 << " + " << previousCarry << " = " << total << " (Result: " << resultDigit << ", Carry: " << nextCarry << ", Cumulative Result: " << cumulative << ")" << std::endl;
}