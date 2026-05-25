#include "../inc/myBigNumber.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Vui long nhap tham so" << '\n';
        return 0;
    }

    string stn1 = argv[1];
    string stn2 = argv[2];

    MyBigNumber myBigNumber;
    string result = myBigNumber.sum(stn1, stn2);

    cout << "Ket qua: " << result << '\n';
    return 0;
}