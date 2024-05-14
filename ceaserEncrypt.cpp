#include <iostream>
#include <string>

using namespace std;

string encrypt(string msg, int shift) {
    string encryptedMsg = "";

    for (int i = 0; i < msg.length(); i++) {
        char c = msg[i];

        if (isalpha(c) && islower(c)) {
            c = ((c - 'a' + shift + 26) % 26) + 'a';
        }
        else if (isalpha(c) && isupper(c)) {
            c = ((c - 'A' + shift + 26) % 26) + 'A';
        }

        encryptedMsg += c;
    }

    return encryptedMsg;
}

int main() {
    string msg;
    int shift;

    cout << "Enter a message: ";
    getline(cin, msg);

    cout << "Enter shift: ";
    cin >> shift;

    cout << "Encrypted Message: " << encrypt(msg, shift) << endl;

    return 0;
}
