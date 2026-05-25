#include "myBigNumber.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Please provide two numbers as command-line arguments." << '\n';
        return 0;
    }

    string stn1 = argv[1];
    string stn2 = argv[2];

    MyBigNumber myBigNumber;
    string result = myBigNumber.sum(stn1, stn2);

    cout << "Result: " << result << '\n';
    return 0;
}